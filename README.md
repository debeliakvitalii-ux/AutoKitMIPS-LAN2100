# AutoKitMIPS — підключення USB-адаптера AutoKit до MIPS-магнітол

**AutoKitMIPS** — програмне забезпечення для підключення сумісного USB-адаптера AutoKit до автомобільної магнітоли з процесором MIPS та Windows CE. Програма має забезпечити реальний зв'язок між адаптером і штатною системою магнітоли для передавання даних CarPlay або Android Auto.

Сфера проєкту — USB-підключення адаптера, протокол обміну, мережева сесія та передавання медіаданих до наявних компонентів магнітоли. Штатний сенсорний екран, фізичні кнопки та керування магнітолою не розробляються й не змінюються AutoKitMIPS.

> **Поточна ціль розробки:** Kia LAN2100 / платформа LGE_AU1350_YN, Windows CE 5.0, MIPSII, Jungo USBware HOST 3.5.14.64. Це не універсальна програма для всіх магнітол і поки що не готовий до встановлення продукт.

## Документація проєкту

- [Повний огляд проєкту та його модулів](docs/PROJECT_OVERVIEW.md)
- [Апаратна сумісність і межі підтримки](docs/HARDWARE_COMPATIBILITY.md)

## Що ми хочемо створити

- Запуск AutoKitMIPS на сумісній автомобільній магнітолі Windows CE.
- Підключення сумісного зовнішнього USB-модуля/адаптера CarPlay або Android Auto.
- Керування життєвим циклом USB-пристрою: підключення, від'єднання, повторне підключення та безпечне завершення операцій.
- Обмін протокольними повідомленнями з адаптером і телефоном.
- Передавання мережевих даних, необхідних для телефонної сесії.
- Виведення відео на екран магнітоли та передавання аудіо — після реалізації відповідних протокольних і апаратних шарів.
- Передавання потоків даних адаптера до наявних мультимедійних компонентів магнітоли. Штатні сенсор, кнопки та інші органи керування залишаються поза сферою проєкту.

Це цільова функціональність, а не перелік уже готових функцій. Статус кожного шару описується в [дорожній карті](docs/IMPLEMENTATION_ROADMAP.md) та [матриці перевірки](docs/VALIDATION_MATRIX.md).

## Для яких магнітол

### Підтверджена первинна платформа

- **Kia LAN2100 / LGE_AU1350_YN**
- Windows CE 5.0
- MIPSII, 32-bit
- Jungo USBware HOST 3.5.14.64
- штатний USB host stack та його JIPOD owner

### Можливі майбутні порти

Архітектурно проєкт може бути адаптований до інших головних пристроїв, якщо вони мають сумісну або документовану платформу: Windows CE, відповідну CPU/ABI, доступний SDK/BSP та сумісний USB host stack.

**Сумісність з іншими Kia/Hyundai, іншими поколіннями LAN, Windows CE-пристроями чи іншими USB-стеками не підтверджена.** Однакова назва ОС або схожий роз'єм не означає сумісності. Для кожної моделі потрібне окреме дослідження й тестування.

Детальніше: [апаратна сумісність і межі підтримки](docs/HARDWARE_COMPATIBILITY.md).

## З яких частин складається програма

1. **Application / UI** — запуск програми, головний екран, налаштування, вибір режимів та індикація стану.
2. **MIPS/OEM adaptation layer** — сумісність із конкретною MIPS-платформою, Windows CE та OEM USB stack/owner.
3. **USB host transport** — реальне керування USB-пристроєм через штатний OEM/Jungo stack.
4. **Protocol/session layer** — встановлення та підтримка сесії з сумісним адаптером і телефоном (зокрема iAP2 або Android Auto/AAP, якщо це відповідає пристрою та ліцензійним вимогам).
5. **Network bridge** — USB networking, IP-конфігурація та необхідні мережеві служби.
6. **Media/data bridge** — передавання потоків даних, аудіо та відео до наявних мультимедійних компонентів магнітоли.
7. **Configuration and diagnostics** — параметри адаптера, стан з'єднання, журнали та відновлення після помилок.

## Поточний стан — чесно

Проєкт перебуває в активній розробці. У репозиторії є вихідні компоненти та host-тести окремих меж програми, але **ще немає підтвердженого повного native USB transport для LAN2100, готової CE5/MIPSII збірки чи робочої CarPlay/Android Auto сесії на магнітолі**.

Ключовий незакритий етап — безпечний виклик через чинного USBware/JIPOD owner і підтвердження справжньої native USB EP0 передачі. Не можна замінювати це вигаданим IOCTL, підробленою відповіддю або другим USB host controller.

- [Огляд стану розробки](docs/DEVELOPMENT_STATUS_2026-11-23.md)
- [Архітектура та правила володіння ресурсами](docs/ARCHITECTURE_AND_LIFETIME_CONTRACT.md)
- [Технічний запит щодо USBware/JIPOD](docs/USB_HELP.md)
- [Дорожня карта](docs/IMPLEMENTATION_ROADMAP.md)

## Кого ми шукаємо

Будемо раді допомозі від:

- Windows CE 5.0 / Platform Builder / eVC4 розробників;
- MIPS/MIPSII та embedded C/C++ інженерів;
- спеціалістів із Jungo USBware, USB host controller, USB protocol та USB device stacks;
- розробників автомобільних мультимедійних систем і OEM BSP;
- інженерів iAP2, Android Auto/AAP, USB networking/NCM;
- спеціалістів із CarPlay/Android Auto протоколів, USB networking/NCM та аудіо/відео потоків;
- тестувальників, які мають сумісну магнітолу або можуть допомогти з відтворюваними тестами.

Не обов'язково знати весь проєкт. Навіть перевірена інформація про конкретну MIPS-магнітолу, OEM SDK, USB owner API або сумісність AutoKit-адаптера може допомогти.

## Як долучитися

1. Прочитайте [CONTRIBUTING.md](CONTRIBUTING.md).
2. Перегляньте [відкриті Issues](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues).
3. Для USBware/JIPOD інтеграції додайте технічну відповідь у [Issue #1](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1).
4. Для нового модуля, виправлення чи документації відкрийте окремий Pull Request із точним описом перевірки.
5. Вкажіть модель обладнання, версію ОС/стека, джерело інформації та що саме було реально протестовано.

## Безпека та правові межі

Не публікуйте закриті OEM SDK, прошивки, дампи пам'яті, ключі, сертифікати, персональні дані або матеріали, які ви не маєте права поширювати. Не виконуйте експерименти з прошивкою, NOR/registry чи USB-пристроями без відповідного плану й дозволу. Не вважайте публічність репозиторію автоматичною ліцензією на сторонні матеріали.

## English

AutoKitMIPS is software intended to connect a compatible AutoKit USB adapter to MIPS-based automotive head units running Windows CE. The first target is Kia LAN2100 / LGE_AU1350_YN (Windows CE 5.0, MIPSII, Jungo USBware HOST 3.5.14.64).

The scope is adapter connectivity, USB transport, session protocols, networking and delivery of media data to the head unit's existing multimedia components. Factory touchscreen and buttons are outside this project. These are project goals, **not claims of completed functionality**. No working native LAN2100 USB transport, target CE5/MIPSII release build, or complete CarPlay/Android Auto session has been verified yet.

We welcome embedded Windows CE, MIPS, USB/Jungo, automotive BSP, networking, media-pipeline and hardware-testing contributors. Start with [the hardware compatibility note](docs/HARDWARE_COMPATIBILITY.md), [contributor guide](CONTRIBUTING.md), and [open issues](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues).

