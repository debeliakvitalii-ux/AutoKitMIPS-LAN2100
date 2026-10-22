# AutoKitMIPS — LAN2100

**Експериментальний проєкт для штатної магнітоли LAN2100 на Windows CE 5.0 / MIPSII.** Мета — автономна робота програми з реальним USB-транспортом до Carlinkit і подальшою інтеграцією CarPlay.

[English](#english) · [P0: відновлення робочої бази](../../issues/2) · [P1: реальний USB provider](../../issues/1)

## Поточний пріоритет — відновити робочу програму

**Оновлено 22 жовтня 2026 року за COORD v21, A v68, B v73 і C v70.**

Власник повідомив про проблеми відкриття/компіляції та роботи нової експериментальної версії. Ручну QUERY-кнопку відхилено. A65/A66 більше не використовуються як активна база відновлення.

Потрібно повернути останню підтверджену робочу поведінку: запуск програми, штатні MGR1/UWD1, OEM receive/session і локальне відео. **Відновлення цієї поведінки не означає готовий CarPlay або фізичний Apple EP0.**

| Напрям | Фактичний стан |
| --- | --- |
| Історична REV26 | Є журнал запуску на магнітолі та локального H.264: 86/86 кадрів, без помилок відображення |
| Оригінальний ZIP REV26 | Точні байти потрібного архіву ще не автентифіковані/не відновлені |
| Новий пакет відновлення | Не доставлений і не прийнятий |
| Нова цільова збірка / HU-тест | Не виконані для відновленого пакета |
| A65/A66 з ручною кнопкою | Відкликані як активна база випуску; історичні source-review висновки збережені |
| Реальний USB provider | Відповідний OEM same-owner EP0 інтерфейс досі відсутній |
| Receiver / NTP / H.264 integration | Паралельна робота після P0; не умова відновлення старого запуску |

## P0 — допомогти відновити точний REV26

[Відкрите завдання №2](../../issues/2)

Потрібен оригінальний архів із SHA-256:

```text
7c437e9dc066640174ba5c6a4838a848ccb8b0f14476ea5f9b27315861288bb9
```

Історична назва: `AutoKitMIPS_v22.8.114_U_LOCAL_H264_SOFTWARE_EXPERIMENTAL.zip`.
**Назва не визначає ревізію:** знайдені однойменні архіви REV24 і REV28 мають інші хеші й не є REV26.

Якщо ви зберегли оригінальний пакет, повідомте його SHA-256 та походження у завданні №2. Не перебудовуйте REV26 з іншої ревізії і не змінюйте файл хешу, щоб він «збігався».

Після отримання правильного оригіналу потрібні:

1. Збереження справжнього дерева `AutoKitMIPS_U`, workspace/project і штатної поведінки без додавання експериментальних модулів.
2. Відсутність ручної кнопки ID 3009 та активного `AutoKitManualUsbQuery.cpp`.
3. Відтворювана збірка у відповідному Windows/eVC4/CE5 MIPSII середовищі: точний SDK, конфігурація, журнал компілятора/лінкера, SHA вихідного архіву й нового EXE.
4. Окремо узгоджена перевірка поведінки на магнітолі.

Історичний запис про USER_BUILD_PASS збережено, але C70 не отримав новий raw compiler/link transcript або точну прив’язку EXE до відновленого пакета. Старий HU-журнал доводить історичну поведінку; він не є новим тестом відновлення.

Якщо REV26 недоступна, інша база може розглядатися лише як **окремий кандидат із власними доказами**, а не як перейменована REV26.

## P1 — реальний автономний USB-транспорт

[Відкрите завдання №1](../../issues/1)

Потрібен реалізовний зв’язок із USB-пристроєм через чинного штатного власника. Зберігаємо UWD1/MGR1 та OEM message registration.

| Параметр | Ціль |
| --- | --- |
| Платформа | LAN2100, Windows CE 5.0, MIPSII, OEM target LGE_AU1350_YN |
| Досліджений stack | Jungo USBware HOST 3.5.14.64, OEM GEN15 production update 8.2.6 |
| Власник USB | Штатний `jipod` / USBware |
| Потрібний компонент | Відповідний збірці app-callable EP0 service або broker усередині поточного owner |
| Початковий control IN | `C0 53 00 00 00 00 04 00` |
| Критерій успіху | Реальне terminal OK, actual_length = 4, отримані `01 00 00 00`, чинна generation |

Це очікуваний результат, **не зафіксована фізична відповідь**. Досліджений образ оновлення не встановлений як byte-identical до поточної системи конкретної магнітоли.

Корисний внесок:

- Оригінальний header **разом із відповідною реалізацією**, бібліотекою або service/IPC contract для саме цієї CE5/MIPSII збірки.
- Конкретний native export/IPC endpoint, prototype, packing, calling convention та походження device handle.
- Контракт attach-generation, owner pin, submit/completion, справжніх status/actualLength/bytes, timeout/cancel і callback drain.
- Новий відповідний артефакт або конкретний контрприклад, який змінює попередній висновок.

Імена application callback на кшталт `query_terminal_sync` не є доказом OEM API. Один header, заглушка або ще одна callback-обгортка не створюють фізичний USB provider.

### Автономність без нової кнопки

Майбутній справжній provider має працювати через підтверджену подію готовності та поточну physical attach generation. Після перевірки ABI, lifetime та окремого погодження hardware-етапу допускається одна QUERY-спроба на generation через того самого owner.

**Зараз цей шлях не активований.** За відсутності provider — нуль submit, без вигаданого ACK, таймера повторів чи непідтвердженого автоматичного USB-виклику. Відновлена програма повинна виконувати свою вже підтверджену штатну поведінку. COMMIT `40/51`, role switch і конкуруючий HCD не входять у поточне відновлення.

## Що вже досліджено

| Робота | Результат та межі |
| --- | --- |
| 13 жовтня: CE6 USBware-бібліотеки | Досліджені MSC/ActiveSync/MTP об’єкти не дають потрібного LAN2100 host EP0 provider |
| 16 жовтня: експорти та реєстрація | Потрібні внутрішні host-функції не підтверджені як доступні експорти дослідженої OEM DLL |
| 16 жовтня: внутрішній loader | Простежений ActivateDevice-шлях стосується mass-storage, не доводить довільний host plugin |
| 16 жовтня: UWD / Phaethon | Логічний канал та OEM status DWORD не є довільним EP0 response / actual length |
| 16 жовтня: старий TI SDK | Reference іншої версії/платформи; не ABI поточної OEM-збірки |
| 19 жовтня: resource producer | Статичний ланцюжок descriptor → context +0x24 → HCD lookup знайдений; це не прикладний USB API і не runtime ownership |

Повторювати ці роботи варто тільки за наявності нового матеріалу або конкретного контрприкладу. Не прирівнюйте stream HANDLE до USB device handle чи статичну адресу до дозволеного runtime-виклику.

## Як долучитися

1. Оберіть [P0](../../issues/2) або [P1](../../issues/1), або відкрийте Issue з конкретним окремим внеском.
2. Укажіть ревізію, файли/SHA, походження матеріалу, очікувану поведінку та що саме додаєте.
3. Узгодьте ділянку роботи в Issue, щоб не дублювати іншого учасника.
4. Зміни коду подавайте через Pull Request з вузькою перевіркою зміненої поведінки.
5. Окремо позначайте статичний аналіз, host-тести, MIPSII-компіляцію та фактичний HU-тест.

Формат дефекту: **ревізія → умови → фактична поведінка → очікувана поведінка → відтворювач**.

Основна інтеграція має зберігати один корінь `AutoKitMIPS_U`, один обраний набір source і правдивий журнал. `ReviewOnly` не означає активований код. Старий EXE та синтетичний ACK не є доказом нового USB-тесту.

Наразі репозиторій містить координаційну документацію; оригінальний REV26 та повне дерево програми тут не опубліковані. Поширюйте лише матеріали, які маєте право передавати, зі збереженням походження та сторонніх ліцензій. Ця сторінка не встановлює нову ліцензію для стороннього коду.

---

## English

### AutoKitMIPS for LAN2100 — contributors wanted

Experimental Windows CE 5.0 / MIPSII work toward autonomous USB transport to a Carlinkit adapter and subsequent CarPlay integration.

**Updated 22 October 2026 from COORD v21, A v68, B v73 and C v70.** The owner reported that the newer experimental project failed to open/build or work reliably, and rejected the manual QUERY button. A65/A66 are withdrawn as active restoration candidates. Their historical source-review results do not establish a working release.

### Priority 0: restore the last known working application

See [issue #2](../../issues/2). We need the authentic REV26 archive with SHA-256:

```text
7c437e9dc066640174ba5c6a4838a848ccb8b0f14476ea5f9b27315861288bb9
```

Historical filename: `AutoKitMIPS_v22.8.114_U_LOCAL_H264_SOFTWARE_EXPERIMENTAL.zip`. Same-named REV24 and REV28 archives are different artifacts, not substitutes.

C70 verified a historical REV26 head-unit log with application startup, MGR1/UWD1, OEM receive/session activity, local H.264 playback (86/86 frames) and clean shutdown. This is historical application/local-file behavior, **not physical Apple EP0, authenticated CarPlay or a new restored-package test**.

The exact original archive remains unavailable for authenticated restoration. No new rollback package has been delivered. The historical user build report does not replace current raw compiler/link evidence or exact package-to-executable provenance.

Useful contributions: recover the original archive; establish source provenance; reproduce the matching eVC4 / CE5 MIPSII build; supply real compiler/link output and a fresh EXE SHA. Preserve original startup and OEM behavior. Do not add manual button ID 3009 or active `AutoKitManualUsbQuery.cpp`. Any fallback must have its own identity and evidence.

### Priority 1: genuine same-owner USB provider

See [issue #1](../../issues/1). Target: **Jungo USBware HOST 3.5.14.64 / Windows CE 5.0 / MIPSII / LGE_AU1350_YN**, studied OEM GEN15 8.2.6 update; existing owner **jipod**. Keep stock UWD1/MGR1 and OEM message registration intact.

Needed: matched application-facing EP0 service or in-owner broker, original header and implementation/library/IPC specification, exact calling convention, real device-handle provenance, attach-generation pin, terminal completion/status/actual length/bytes, timeout/cancel and callback drain.

The future transport must be autonomous, without a manual QUERY UI. Only after a genuine provider is ready, its ABI/lifetime is reviewed and hardware work is separately agreed may it arm one control IN per current physical attach generation:
`C0 53 00 00 00 00 04 00`.

Expected success requires real terminal OK, exactly four received bytes `01 00 00 00` and a still-valid generation. No physical success is claimed. With no provider, submit count stays zero; do not create fake ACKs, polling retries or unsupported automatic calls. COMMIT/role switching and a competing HCD are outside the restoration task.

### Avoid duplicate research; contribute evidence

The existing CE6 library, OEM exports/loader, UWD/Phaethon, older TI SDK and static resource-producer investigations are summarized above. Reopen them with new matched artifacts or concrete counterexamples, not a renamed task. A stream HANDLE is not a USB device handle; OEM command status is not USB response data.

Use Issues for scoped work and Pull Requests for code. Record exact revision and provenance. Distinguish source review, host tests, native compilation and hardware execution. Receiver/NTP/H.264 integration is separate from P0 restoration.

This repository currently hosts coordination documentation, not the authenticated REV26 archive or a complete published source tree. Share only material you are entitled to distribute, preserving third-party notices and licenses.
