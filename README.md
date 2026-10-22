# AutoKitMIPS — LAN2100

**Експериментальний проєкт AutoKitMIPS для штатної магнітоли LAN2100 на Windows CE 5.0 / MIPSII.**

Мета — довести програму до реального USB-транспорту й подальшої роботи з адаптером CarPlay. Шукаємо учасників із досвідом Windows CE, MIPS, Jungo USBware та вбудованих USB host-драйверів.

**Поточний стан:** перевірений пакет вихідного коду існує; підтвердженого фізичного USB QUERY та готового USB-тестового EXE для цього пакета ще немає. Приймання вихідного коду або успішний тест на ПК не означає роботу на магнітолі.

[English version](#english)

## Найважливіша допомога

Потрібен реалізовний зв’язок між програмою та USB-пристроєм, яким уже володіє штатний Jungo USBware. Ще одна обгортка з callback-функціями цю прогалину не закриває.

| Параметр | Підтверджений контекст |
| --- | --- |
| Платформа | LAN2100, Windows CE 5.0, MIPSII; OEM target LGE_AU1350_YN |
| Досліджений host stack | Jungo USBware HOST 3.5.14.64, OEM GEN15 production update 8.2.6 |
| Обраний напрям | Робота через поточного штатного власника USB, без конкуруючого HCD |
| Відсутній компонент | Відповідний цій збірці application-facing EP0 API/service або достатні вихідні матеріали для його реалізації |
| Перший цільовий запит | Control IN: `C0 53 00 00 00 00 04 00` |
| Критерій відповіді | Реальне terminal OK, actual_length = 4, отримані байти `01 00 00 00` |

Дослідження виконані на образі оновлення. Його відповідність байт у байт поточній системі на конкретній магнітолі не встановлена.

Корисний внесок для USB — один із таких:

- Документація та реалізація інтерфейсу саме USBware HOST 3.5.14.64 / CE5 / MIPSII: header разом із відповідною бібліотекою, service/export/IPC contract або вихідним кодом.
- Матеріали OEM/Jungo, які пояснюють реєстрацію host-клієнта, отримання device context, attach/detach, completion, timeout, cancel та завершення callback перед звільненням ресурсів.
- Відтворюваний аналіз нового відповідного артефакту, який встановлює конкретний виклик і справжнє походження owner/device handle.

Для кожного матеріалу вкажіть версію, походження, назву файла та SHA-256. Поширюйте лише матеріали, на передачу яких маєте право. Один header без відповідної реалізації або підтвердженого ABI не закриває залежність.

## Коротка карта проєкту

Це функціональні частини, а не твердження про вже активований наскрізний шлях.

| Частина | Стан / потрібна робота |
| --- | --- |
| AutoKitMIPS_U, основна програма | Є повний EXPERIMENTAL source package |
| Ручний QUERY-вхід | A64 доставив source-only функцію та owner-release корекцію; незалежне приймання очікується, активний VCP не змінено |
| Фізичний USB provider | Основна відкрита залежність, описана вище |
| Owner / generation / drain | Коректність при від’єднанні, timeout і одночасному завершенні |
| Receiver / NTP / H.264 | B68 узгодив одну C-прийняту NTP-реалізацію; зовнішній lifetime receiver ще потребує інтеграції. Не залежність першого QUERY |
| CE5 MIPSII build | Потрібен справжній результат eVC4/CLMIPS для актуальних змін |
| Тест на магнітолі | Окремий етап після підготовки конкретного пакета |

## Що вже досліджено — не починайте заново

| Робота | Результат та межі висновку |
| --- | --- |
| 13 жовтня 2026: наявні CE6 USBware-бібліотеки | Досліджені MSC/ActiveSync/MTP об’єкти не дають потрібного LAN2100 host EP0 provider |
| 16 жовтня: експорти та реєстрація USBware | Потрібні внутрішні host-функції не підтверджені як доступні експорти дослідженої OEM DLL |
| 16 жовтня: внутрішній loader | Простежений ActivateDevice-шлях стосується mass-storage; він не доводить завантаження довільного host-клієнта |
| 16 жовтня: UWD / Phaethon | Наявний логічний канал і OEM status DWORD не є довільним EP0 response / actual length |
| 16 жовтня: старий TI USBware SDK | Корисний reference іншої версії та платформи; не ABI поточної OEM-збірки |
| 19 жовтня: resource producer | Статичний ланцюжок descriptor → device context +0x24 → HCD lookup знайдений; це не прикладний USB API й не доказ runtime ownership |

Ці висновки обмежені дослідженими версіями та файлами. Нові байти, інша підтверджена збірка або конкретний контрприклад можуть змінити висновок. Повтор тієї самої перевірки без нового входу не є новим завданням.

## Як долучитися

1. Відкрийте Issue з конкретним внеском: новий OEM/API матеріал, дефект коду, результат цільової збірки або пропозиція реалізації.
2. Назвіть вихідну ревізію/commit або SHA файла, очікувану поведінку та що саме зміниться.
3. Узгодьте вузьку ділянку роботи в Issue, щоб учасники не дублювали одне одного.
4. Для зміни коду підготуйте Pull Request із реалізацією та перевіркою конкретного ризику.
5. Розрізняйте результат аналізу, host-тест, цільову MIPSII-компіляцію та фізичний USB-тест.

Для повідомлення про дефект достатньо: **файл/ревізія → умови → фактична поведінка → очікувана поведінка → відтворювач**.

Для USB-внеску додайте: **версія stack → реальний інтерфейс → походження handle → submit/completion → cancel/drain/lifetime**.

## Правила інтеграції

- Основний корінь програми: `AutoKitMIPS_U`. Інтеграція має продовжувати наявний код, а не створювати паралельні реалізації тієї самої функції.
- `ReviewOnly` означає неактивний кандидат. Його наявність у пакеті не означає підключення до активного VCP або HCD.
- Не прирівнюйте stream HANDLE до USB device handle, OEM status DWORD до USB-відповіді, статичну адресу до підтвердженого runtime ownership.
- Перший USB milestone — лише ручний одноразовий QUERY. COMMIT `40/51`, role switch, встановлення драйвера та MMIO не входять у цей milestone.
- Фізичні дії на конкретній магнітолі потребують окремого погодженого плану з її власником.
- Не публікуйте синтетичний ACK або старий EXE як доказ нового фізичного USB-тесту.
- Вихідні зміни йдуть через історію Git; повні ZIP потрібні для визначених пакетів, без тимчасових копій `(1)`, `(2)`.

## Базова ревізія та перевірка збірки

Опис стану підготовлено за A_USB_STATUS v62 (доставка A64), B_USB_STATUS v68 і C_USB_STATUS v64, прочитаними **22 жовтня 2026 року**. C64 відхилив попередню fresh A59 через гонку release/withdrawal; пізнішу A64 ще не прийнято незалежно. A повідомляє нові host-тести 46/46 на GCC і Clang, без native/HU запуску. B68 узгодив C62-прийняту NTP-реалізацію SHA `597b5e610ff2642378fc8cacb80d8eb86e7d3231548e73bccb0407d4247fd3a5`. Основний ZIP нижче поки не змінений.

Базовий пакет:
`AutoKitMIPS_v22.8.114_U_SRCREV35_B44_B46_FULL_SOURCE_ONLY_EXPERIMENTAL_20261022.zip`

SHA-256:
`6377ee6e19fe25a15ec3dc7c2efb3c8a687a60d71e7532f31779b0ae13f47f0b`

За звітом C63: структура та маніфест пакета перевірені; B44/B46 включені тільки як неактивний ReviewOnly source. Нового MIPSII EXE в цьому пакеті немає. Це ідентифікатор базового матеріалу, а не посилання на опублікований GitHub Release.

Для учасника, який уже має цей пакет і відповідне Windows/eVC4/CE5 MIPSII середовище: наявний `BUILD_EVC4_MIPSII_RELEASE.cmd` запускається з кореня `AutoKitMIPS_U` із правильно вибраним SDK. Передайте `BuildEvidence/EVC4_MIPSII_Release_Last.log`, результат компілятора/лінкера та SHA нового EXE. Наявний `verify_mipsii_output.py` перевіряє очікуваний COFF machine `0x0166`; це перевірка формату, не запуск на HU.

Ця сторінка є точкою координації. Публікація повного вихідного дерева та його ліцензій має бути окремо узгоджена; цей опис не встановлює нову ліцензію для сторонніх компонентів.

---

## English

### AutoKitMIPS for LAN2100 — contributors wanted

AutoKitMIPS is an experimental effort targeting a LAN2100 head unit running **Windows CE 5.0 / MIPSII**. The goal is real USB transport followed by integration with a CarPlay adapter.

**Status as recorded on 22 October 2026:** a reviewed full source package exists. A genuine physical USB QUERY and a newly built USB-test executable have not been demonstrated for that package. Host tests and source review are not hardware results.

### The contribution that would unblock USB

We need a build-matched application-facing EP0 interface, or enough authentic implementation material to create one within the existing owner:

- **Jungo USBware HOST 3.5.14.64**
- **Windows CE 5.0 / MIPSII / LGE_AU1350_YN**
- Studied firmware lineage: **OEM GEN15 production update 8.2.6**
- Existing native USB owner: **jipod / USBware**

A useful delivery identifies the real exported function, service or IPC endpoint; its prototype and packing; device/context acquisition; attach-generation lifetime; actual submission; terminal USB status, actual length and response bytes; timeout, cancellation and callback drain.

The initial milestone is one manually requested control IN:
`C0 53 00 00 00 00 04 00`.
A successful result requires real terminal success, exactly four received bytes and `01 00 00 00`. This is an expected result, not an observed result.

The project already has application callback slots. Implementing another placeholder interface does not provide hardware access. The alternative HCD route also requires proven exclusive controller ownership and matching BSP integration; known static register values alone do not establish that.

### Work already completed

Do not restart the existing CE6 library classification, OEM export/loader investigation, UWD/Phaethon command analysis, old TI reference SDK analysis or static resource producer trace without new evidence. The static resource producer was located on 19 October; it is not the missing application EP0 service.

Existing UWD command status is not USB response data. A stream HANDLE is not a USB device handle. Recovery/CE6/ARM components and other USBware versions are not automatically compatible with this target.

### Latest software progress

A64 has delivered a source-only manual QUERY function and an owner-release correction, reporting 46/46 focused host assertions on each of GCC and Clang. Independent acceptance is pending; this code is not selected in the active VCP. C64 rejected the preceding fresh A59 release/withdrawal race, not the later A64 delivery. B68 reconciled the NTP selection to the already C62-accepted implementation. Receiver outer-owner integration and the physical USB dependency remain open. The full baseline archive is unchanged.

### Other useful contributions

- A reproducible eVC4 / CE5 MIPSII build of the selected source, with the first actual compiler/linker failure or a fresh executable and build log.
- Focused fixes for ownership withdrawal during drain/release and callback lifetime.
- Receiver teardown, NTP lifecycle and H.264 integration work in parallel; these are not prerequisites for the first USB QUERY.

Open an Issue with your exact revision, proposed contribution and evidence. Submit focused code changes through a Pull Request. Clearly distinguish static analysis, host tests, target compilation and hardware observations. Preserve source provenance and third-party notices; share only material you are entitled to distribute.

No automatic COMMIT, role switch, driver installation or MMIO is part of the initial QUERY milestone. Hardware work must be separately agreed with the device owner.

The baseline package name and SHA-256 are listed above. This coordination page does not claim the source archive is already a GitHub release or assign a new license to its dependencies.

