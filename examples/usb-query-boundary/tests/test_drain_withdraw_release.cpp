#include "akusb01_query_terminal_gate.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>

static int passed = 0, failed = 0;
#define CHECK(expr) do { if (expr) ++passed; else { ++failed; fprintf(stderr,"FAIL %s:%d %s\n",__FILE__,__LINE__,#expr); } } while (0)
struct Harness {
    pthread_mutex_t dispatcher;
    pthread_mutex_t signals;
    pthread_cond_t release_signal;
    AK_USBWARE_SYNC_EP0_GENERATION* generation;
    DWORD number;
    int alive, issues, drains, releases, lock_enters, lock_leaves;
    int inside_dispatch, foreign_release, released_signal;
    int withdraw_in_drain, withdraw_inside_release, withdraw_on_dispatch_enter;
    int fail_release;
};
static void init_h(Harness* h,AK_USBWARE_SYNC_EP0_GENERATION* gen, DWORD number) {
    memset(h,0,sizeof(*h));
    pthread_mutex_init(&h->dispatcher,0);
    pthread_mutex_init(&h->signals,0);
    pthread_cond_init(&h->release_signal,0);
    h->generation=gen;h->number=number;h->alive=1;
}
static void close_h(Harness* h) {
    pthread_cond_destroy(&h->release_signal);
    pthread_mutex_destroy(&h->signals);
    pthread_mutex_destroy(&h->dispatcher);
}
static void init_generation(AK_USBWARE_SYNC_EP0_GENERATION* g,DWORD n) {
    memset(g,0,sizeof(*g));
    CHECK(UsbwareSyncEp0Query_Init(g));
    CHECK(UsbwareSyncEp0Query_BeginGeneration(g,n,(void*)0x1234,(void*)0x5678,TRUE));
}
static int owner(void* user,const AK_USBWARE_SYNC_EP0_GENERATION*,DWORD) {
    return ((Harness*)user)->alive;
}
static BOOL issue(void* user,void*,const unsigned char setup[8],AK_USBWARE_SYNC_COMPLETION* completion) {
    Harness* h=(Harness*)user;
    static const unsigned char query[]={0xC0,0x53,0,0,0,0,4,0};
    CHECK(memcmp(setup,query,8)==0);
    ++h->issues;
    completion->terminal=TRUE;
    completion->state=AK_USBWARE_TERMINAL_SUCCESS;
    completion->actualLength=4;
    completion->nativeStatus=0xBEEF;
    completion->reply[0]=1;completion->reply[1]=0;completion->reply[2]=0;completion->reply[3]=0;
    return TRUE;
}
static int drain(void* user,void*,DWORD) {
    Harness* h=(Harness*)user;
    ++h->drains;
    if (h->withdraw_in_drain) {
        h->alive=0;
        CHECK(UsbwareSyncEp0Query_WithdrawOwner(h->generation,h->number));
    }
    return 1;
}
static int enter(void* user,const AK_USBWARE_SYNC_EP0_GENERATION*,DWORD) {
    Harness* h=(Harness*)user;
    pthread_mutex_lock(&h->dispatcher);
    ++h->lock_enters;
    h->inside_dispatch=1;
    if (h->withdraw_on_dispatch_enter) {
        h->alive=0;
        CHECK(UsbwareSyncEp0Query_WithdrawOwner(h->generation,h->number));
    }
    return 1;
}
static void leave(void* user,const AK_USBWARE_SYNC_EP0_GENERATION*,DWORD) {
    Harness* h=(Harness*)user;
    ++h->lock_leaves;
    h->inside_dispatch=0;
    pthread_mutex_unlock(&h->dispatcher);
}
static int release(void* user,void*,DWORD) {
    Harness* h=(Harness*)user;
    ++h->releases;
    if (!h->inside_dispatch || !h->alive) ++h->foreign_release;
    pthread_mutex_lock(&h->signals);
    h->released_signal=1;
    pthread_cond_broadcast(&h->release_signal);
    pthread_mutex_unlock(&h->signals);
    if (h->withdraw_inside_release) {
        h->alive=0;
        CHECK(UsbwareSyncEp0Query_WithdrawOwner(h->generation,h->number));
    }
    return !h->fail_release;
}
static AKUSB01_EXACT_OWNER_BINDING binding(Harness* h) {
    AKUSB01_EXACT_OWNER_BINDING b;
    memset(&b,0,sizeof(b));
    b.user=h;b.verified_current_owner=owner;b.query_terminal_sync=issue;
    b.drain_verified=drain;b.release_owned=release;
    b.owner_dispatch_enter=enter;b.owner_dispatch_leave=leave;
    return b;
}
static void* withdraw_thread(void* arg) {
    Harness* h=(Harness*)arg;
    pthread_mutex_lock(&h->signals);
    while (!h->released_signal) pthread_cond_wait(&h->release_signal,&h->signals);
    pthread_mutex_unlock(&h->signals);
    /* The true external dispatcher serializes physical generation revocation
     * against release_owned and the status publication. */
    pthread_mutex_lock(&h->dispatcher);
    h->alive=0;
    UsbwareSyncEp0Query_WithdrawOwner(h->generation,h->number);
    pthread_mutex_unlock(&h->dispatcher);
    return 0;
}
int main() {
    AK_USBWARE_SYNC_EP0_GENERATION g,newer;
    AKUSB01_QUERY_OBSERVATION out;
    AKUSB01_QUERY_STATUS status;
    AKUSB01_EXACT_OWNER_BINDING b;
    Harness h;
    pthread_t t;

    /* No provider => zero submissions and no one-shot consumed. */
    init_generation(&g,10);
    status=akusb01_manual_query_once(&g,10,0,&out);
    CHECK(status==AKUSB01_BACKEND_UNAVAILABLE && !out.issued && !g.queryAttempted);
    init_h(&h,&g,10);b=binding(&h);b.owner_dispatch_enter=0;
    CHECK(akusb01_manual_query_once(&g,10,&b,&out)==AKUSB01_BACKEND_UNAVAILABLE);
    CHECK(h.issues==0 && h.releases==0 && !g.queryAttempted);
    close_h(&h);

    /* Owner withdraws *inside drain*: no release of a foreign pointer,
     * no positive ACK, still only one attempted submit. */
    init_generation(&g,11);init_generation(&newer,12);
    init_h(&h,&g,11);h.withdraw_in_drain=1;b=binding(&h);
    status=akusb01_manual_query_once(&g,11,&b,&out);
    CHECK(status==AKUSB01_OWNER_RETAINED_UNDRAINED);
    CHECK(h.issues==1 && h.drains==1 && h.releases==0 && !h.foreign_release);
    CHECK(!g.queryAck && g.retiring && g.queryAttempted && out.response[0]==0);
    CHECK(newer.generation==12 && newer.attachReady && !newer.retiring);
    CHECK(akusb01_manual_query_once(&g,11,&b,&out)!=AKUSB01_ACK_RELEASED_SOURCE_ONLY);
    CHECK(h.issues==1);close_h(&h);

    /* Owner disappears on entering final dispatch gate (after drain) =>
     * atomic final check refuses release before it can begin. */
    init_generation(&g,13);init_h(&h,&g,13);h.withdraw_on_dispatch_enter=1;
    b=binding(&h);status=akusb01_manual_query_once(&g,13,&b,&out);
    CHECK(status==AKUSB01_OWNER_RETAINED_UNDRAINED);
    CHECK(h.issues==1 && h.drains==1 && h.releases==0);
    CHECK(h.lock_enters==1 && h.lock_leaves==1 && !h.foreign_release);
    CHECK(!g.queryAck && out.response[0]==0);close_h(&h);

    /* Reentrant withdrawal during the release callback is latched by the
     * backend stateLock; EndReleaseLease must refuse to publish positive ACK. */
    init_generation(&g,14);init_h(&h,&g,14);h.withdraw_inside_release=1;
    b=binding(&h);status=akusb01_manual_query_once(&g,14,&b,&out);
    CHECK(status!=AKUSB01_ACK_RELEASED_SOURCE_ONLY);
    CHECK(h.issues==1 && h.drains==1 && h.releases==1);
    CHECK(out.response[0]==0 && !g.queryAck && !g.releaseLeaseActive);
    CHECK(g.withdrawalPending && g.queryAttempted && g.retiring);
    close_h(&h);

    /* Parallel detach blocks at dispatcher until owner release/result
     * publication completes. No foreign release and no resubmit possible. */
    init_generation(&g,15);init_h(&h,&g,15);b=binding(&h);
    CHECK(pthread_create(&t,0,withdraw_thread,&h)==0);
    status=akusb01_manual_query_once(&g,15,&b,&out);
    CHECK(status==AKUSB01_ACK_RELEASED_SOURCE_ONLY);
    CHECK(out.issued==1 && out.drained==1 && out.released==1);
    CHECK(out.query.terminal && out.query.actualLength==4 && out.query.nativeStatus==0xBEEF);
    CHECK(!memcmp(out.response,"\x01\x00\x00\x00",4));
    CHECK(pthread_join(t,0)==0);
    CHECK(h.issues==1 && h.releases==1 && !h.foreign_release);
    CHECK(!g.queryAck && g.retiring && g.queryAttempted && !g.releaseLeaseActive);
    CHECK(akusb01_manual_query_once(&g,15,&b,&out)!=AKUSB01_ACK_RELEASED_SOURCE_ONLY);
    CHECK(h.issues==1);close_h(&h);

    printf("HOST_QUERY_BOUNDARY_ASSERTIONS %d/%d %s\n",passed,passed+failed,failed?"FAIL":"PASS");
    return failed?1:0;
}
