# AutoKitMIPS — автомобільна мультимедійна платформа для Windows CE

**AutoKitMIPS** — відкритий інженерний проєкт зі створення програмної платформи, яка має дати змогу використовувати сучасні телефонні інтерфейси та сумісні USB-модулі CarPlay / Android Auto на автомобільних мультимедійних головних пристроях старого покоління, які працюють під Windows CE.

Мета — не просто виправити одну помилку USB. Ми створюємо повний програмний міст між автомобільною магнітолою, USB-пристроєм/адаптером, телефоном та інтерфейсом водія: підключення, обмін даними, мережа, звук, зображення та керування — у межах того, що реально підтримує конкретне обладнання.

> **Поточна ціль розробки:** Kia LAN2100 / платформа LGE_AU1350_YN, Windows CE 5.0, MIPSII, Jungo USBware HOST 3.5.14.64. Це не універсальна програма для всіх магнітол і поки що не готовий до встановлення продукт.

## Що ми хочемо створити

- Запуск AutoKitMIPS на сумісній автомобільній магнітолі Windows CE.
- Підключення сумісного зовнішнього USB-модуля/адаптера CarPlay або Android Auto.
- Керування життєвим циклом USB-пристрою: підключення, від'єднання, повторне підключення та безпечне завершення операцій.
- Обмін протокольними повідомленнями з адаптером і телефоном.
- Передавання мережевих даних, необхідних для телефонної сесії.
- Виведення відео на екран магнітоли та передавання аудіо — після реалізації відповідних протокольних і апаратних шарів.
- Передавання команд керування: сенсорний екран, резистивний touch, фізичні кнопки магнітоли та/або кермо — лише якщо конкретна магнітола надає документований доступ до цих сигналів.
- Налаштування роздільної здатності, орієнтації, масштабу та прив'язки елементів керування до можливостей конкретного дисплея.

Це цільова функціональність, а не перелік уже готових функцій. Статус кожного шару описується в [дорожній карті](docs/IMPLEMENTATION_ROADMAP.md) та [матриці перевірки](docs/VALIDATION_MATRIX.md).

## Для яких магнітол

### Підтверджена первинна платформа

- **Kia LAN2100 / LGE_AU1350_YN**
- Windows CE 5.0
- MIPSII, 32-bit
- Jungo USBware HOST 3.5.14.64
- штатний USB host stack та його JIPOD owner

### Можливі майбутні порти

Архітектурно проєкт може бути адаптований до інших головних пристроїв, якщо вони мають сумісну або документовану платформу: Windows CE, відповідну CPU/ABI, доступний SDK/BSP, USB host stack та OEM-механізми роботи з дисплеєм, звуком і кнопками.

**Сумісність з іншими Kia/Hyundai, іншими поколіннями LAN, Windows CE-пристроями чи іншими USB-стеками не підтверджена.** Однакова назва ОС або схожий роз'єм не означає сумісності. Для кожної моделі потрібне окреме дослідження й тестування.

Детальніше: [апаратна сумісність і межі підтримки](docs/HARDWARE_COMPATIBILITY.md).

## З яких частин складається програма

1. **Application / UI** — запуск програми, головний екран, налаштування, вибір режимів та індикація стану.
2. **Hardware adaptation layer** — прив'язка до конкретної магнітоли, дисплея, аудіопристроїв, кнопок і системних сервісів.
3. **USB host transport** — реальне керування USB-пристроєм через штатний OEM/Jungo stack.
4. **Protocol/session layer** — встановлення та підтримка сесії з сумісним адаптером і телефоном (зокрема iAP2 або Android Auto/AAP, якщо це відповідає пристрою та ліцензійним вимогам).
5. **Network bridge** — USB networking, IP-конфігурація та необхідні мережеві служби.
6. **Media bridge** — аудіо, відео та синхронізація.
7. **Input/control bridge** — сенсор, резистивний touch, кнопки та команди водія.
8. **Configuration and diagnostics** — налаштування під конкретну модель, журнали, діагностика та безпечне відновлення після помилок.

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
- спеціалістів з аудіо/відео pipeline, framebuffer, touchscreens та input devices;
- тестувальників, які мають сумісну магнітолу або можуть допомогти з відтворюваними тестами.

Не обов'язково знати весь проєкт. Навіть перевірена інформація про конкретну модель магнітоли, SDK, USB owner API, дисплей або аудіовихід може допомогти.

## Як долучитися

1. Прочитайте [CONTRIBUTING.md](CONTRIBUTING.md).
2. Перегляньте [відкриті Issues](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues).
3. Для USBware/JIPOD інтеграції додайте технічну відповідь у [Issue #1](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1).
4. Для нового модуля, виправлення чи документації відкрийте окремий Pull Request із точним описом перевірки.
5. Вкажіть модель обладнання, версію ОС/стека, джерело інформації та що саме було реально протестовано.

## Безпека та правові межі

Не публікуйте закриті OEM SDK, прошивки, дампи пам'яті, ключі, сертифікати, персональні дані або матеріали, які ви не маєте права поширювати. Не виконуйте експерименти з прошивкою, NOR/registry чи USB-пристроями без відповідного плану й дозволу. Не вважайте публічність репозиторію автоматичною ліцензією на сторонні матеріали.

## English

AutoKitMIPS is an open engineering project aiming to bring modern phone-projection connectivity to selected legacy Windows CE automotive head units. The first target is the Kia LAN2100 / LGE_AU1350_YN platform (Windows CE 5.0, MIPSII, Jungo USBware HOST 3.5.14.64).

The long-term scope includes USB transport, adapter/session protocols, networking, audio/video output, touchscreen and button input, and model-specific configuration. These are project goals, **not claims of completed functionality**. No working native LAN2100 USB transport, target CE5/MIPSII release build, or complete CarPlay/Android Auto session has been verified yet.

We welcome embedded Windows CE, MIPS, USB/Jungo, automotive BSP, networking, media-pipeline and hardware-testing contributors. Start with [the hardware compatibility note](docs/HARDWARE_COMPATIBILITY.md), [contributor guide](CONTRIBUTING.md), and [open issues](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues).

