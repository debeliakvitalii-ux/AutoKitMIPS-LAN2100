# AutoKitMIPS — підключення USB-адаптера AutoKit до MIPS-магнітол

**AutoKitMIPS** — програма для підключення сумісного USB-адаптера AutoKit до автомобільної магнітоли з процесором MIPS та Windows CE. Її призначення — забезпечити програмний шлях між адаптером і наявними компонентами магнітоли для обміну даними та роботи сумісної CarPlay або Android Auto сесії.

Сфера проєкту обмежена самою програмою AutoKitMIPS і її взаємодією з AutoKit USB-адаптером: USB transport через наявний OEM USB host stack/owner, протокол адаптера, сесія, мережевий обмін і передавання аудіо/відеоданих до наявних мультимедійних програмних компонентів магнітоли. Фабричний інтерфейс магнітоли та її органи керування не змінюються.

> **Поточна ціль розробки:** Kia LAN2100 / платформа LGE_AU1350_YN, Windows CE 5.0, MIPSII, Jungo USBware HOST 3.5.14.64. Це цільова платформа, а не підтвердження повної сумісності чи готовий до встановлення продукт.

## Документація проєкту

- [Повний огляд проєкту та його модулів](docs/PROJECT_OVERVIEW.md)
- [Апаратна сумісність і межі підтримки](docs/HARDWARE_COMPATIBILITY.md)

## Що має робити AutoKitMIPS

- Запускатися як окрема програма на сумісній автомобільній магнітолі Windows CE.
- Виявляти сумісний USB-адаптер AutoKit і взаємодіяти з ним через наявний USB host stack магнітоли.
- Безпечно обробляти підключення, від'єднання, повторне підключення та завершення USB операцій.
- Обмінюватися з адаптером потрібними протокольними повідомленнями.
- Підтримувати сумісну CarPlay або Android Auto сесію, якщо це підтверджено для конкретної моделі адаптера.
- Передавати потрібний мережевий трафік і потоки аудіо/відеоданих до наявних мультимедійних програмних компонентів магнітоли, якщо їхні інтерфейси це дозволяють.

Це цільова функціональність, а не перелік готових можливостей. Статус кожного шару описується в [дорожній карті](docs/IMPLEMENTATION_ROADMAP.md) та [матриці перевірки](docs/VALIDATION_MATRIX.md).

## Цільова апаратна платформа

### Первинна ціль

- **Kia LAN2100 / LGE_AU1350_YN**
- Windows CE 5.0
- MIPSII, 32-bit
- Jungo USBware HOST 3.5.14.64
- наявний штатний USB host stack та його JIPOD owner
- сумісний USB-адаптер AutoKit

Це перша платформа для розробки. Її перелік не означає, що повний зв'язок із AutoKit адаптером уже працює на фізичній магнітолі.

### Можливі майбутні MIPS-порти

Інші MIPS-магнітоли можуть розглядатися окремо після перевірки CPU/ABI, версії Windows CE, OEM USB stack, доступу до реального USB device owner, адаптера та мультимедійних програмних інтерфейсів.

**Сумісність з іншими моделями Kia/Hyundai, іншими поколіннями LAN, Windows CE-пристроями чи іншими USB-стеками не підтверджена.** Однакова назва ОС або CPU не доводить сумісності.

Детальніше: [апаратна сумісність і межі підтримки](docs/HARDWARE_COMPATIBILITY.md).

## Програмні шари

1. **AutoKitMIPS application** — запуск програми, її власний екран стану/діагностики та налаштування підключення адаптера.
2. **MIPS / Windows CE adaptation** — сумісність програми з конкретною MIPS-платформою, CE та OEM USB stack.
3. **USB transport** — реальне звернення до адаптера через наявний OEM/Jungo USB owner.
4. **Adapter protocol and session** — протокольний обмін і встановлення сумісної CarPlay або Android Auto сесії.
5. **Networking** — передавання мережевих даних, потрібних сесії адаптера та телефону.
6. **Audio/video data transport** — передавання медіапотоків до наявних мультимедійних програмних компонентів магнітоли через підтверджені інтерфейси.
7. **Adapter diagnostics** — стан USB-підключення й сесії, журнали та відновлення після помилок.

Власний екран AutoKitMIPS для діагностики та налаштувань не змінює фабричний UI магнітоли.

## Що не входить до проєкту

AutoKitMIPS не розробляє, не замінює, не налаштовує та не інтегрує:
- touchscreen або resistive touch panel;
- фізичні кнопки магнітоли чи кнопки керма;
- mapping або конфігурацію кнопок;
- зовнішні дисплеї та зовнішні display/video interfaces;
- aftermarket interface boxes;
- фабричний інтерфейс, меню чи систему керування магнітоли.

Проєкт використовує лише наявні в магнітолі мультимедійні програмні компоненти, якщо їхні інтерфейси сумісні та підтверджені.

## Поточний стан — чесно

Проєкт перебуває в активній розробці. У репозиторії є вихідні компоненти та host-тести окремих програмних меж, але **ще немає підтвердженого повного native USB transport для LAN2100, готової CE5/MIPSII збірки чи робочої CarPlay/Android Auto сесії на магнітолі**.

Ключова невирішена залежність — підтверджений доступ програми до чинного USBware/JIPOD owner і реальна native USB EP0 передача з коректним завершенням та життєвим циклом. Її не можна замінювати вигаданим IOCTL, синтетичною відповіддю або другим USB host controller.

- [Огляд стану розробки](docs/DEVELOPMENT_STATUS_2026-11-23.md)
- [Архітектура та правила володіння ресурсами](docs/ARCHITECTURE_AND_LIFETIME_CONTRACT.md)
- [Технічний запит щодо USBware/JIPOD](docs/USB_HELP.md)
- [Дорожня карта](docs/IMPLEMENTATION_ROADMAP.md)

## Кого ми шукаємо

Будемо раді допомозі від:
- Windows CE 5.0 / Platform Builder / eVC4 розробників;
- MIPS/MIPSII та embedded C/C++ інженерів;
- спеціалістів із Jungo USBware, USB host stack та USB device protocols;
- інженерів OEM BSP, USB transport і життєвого циклу пристроїв;
- розробників протоколів AutoKit, iAP2 або Android Auto/AAP;
- спеціалістів із USB networking/NCM та аудіо/відео data transport;
- тестувальників із сумісною MIPS-магнітолою та AutoKit USB-адаптером.

Корисними будуть перевірені відомості про конкретну MIPS-платформу, OEM USB owner API, модель AutoKit адаптера або відтворювані результати тестів.

## Як долучитися

1. Прочитайте [CONTRIBUTING.md](CONTRIBUTING.md).
2. Перегляньте [відкриті Issues](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues).
3. Для USBware/JIPOD інтеграції додайте технічну відповідь у [Issue #1](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1).
4. Для нового програмного модуля, виправлення чи документації відкрийте окремий Pull Request із точним описом перевірки.
5. Вкажіть модель AutoKit адаптера, модель магнітоли, версії ОС/USB stack, джерела інформації та що саме було реально протестовано.

## Безпека та правові межі

Не публікуйте закриті OEM SDK, прошивки, дампи пам'яті, ключі, сертифікати, персональні дані або матеріали, які ви не маєте права поширювати. Не виконуйте експерименти з прошивкою, NOR/registry чи USB-пристроями без відповідного плану й дозволу. Не вважайте публічність репозиторію автоматичною ліцензією на сторонні матеріали.

## English

AutoKitMIPS is software for connecting a compatible AutoKit USB adapter to a MIPS-based automotive head unit running Windows CE. Its scope is the application, the adapter's real USB transport through the existing OEM USB owner, adapter protocol/session handling, networking, and audio/video data delivery to the head unit's existing multimedia software components.

The project excludes factory touchscreens and resistive touch panels, physical and steering-wheel buttons, button mapping, external displays/display interfaces, aftermarket interface boxes, and modifications to the factory head-unit UI. AutoKitMIPS may provide its own adapter status and configuration screen.

The first development target is Kia LAN2100 / LGE_AU1350_YN (Windows CE 5.0, MIPSII, Jungo USBware HOST 3.5.14.64). These are project goals, **not claims of completed functionality**. No working native LAN2100 USB transport, target CE5/MIPSII release build, or complete CarPlay/Android Auto session has been verified yet.

We welcome contributors experienced in Windows CE, MIPS, Jungo USB, AutoKit adapter protocols, networking/NCM, and audio/video data transport. Start with [the hardware compatibility note](docs/HARDWARE_COMPATIBILITY.md), [contributor guide](CONTRIBUTING.md), and [open issues](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues).
