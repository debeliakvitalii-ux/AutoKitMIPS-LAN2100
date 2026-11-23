# AutoKitMIPS — LAN2100

Автономне підключення Carlinkit / CarPlay до магнітоли LAN2100 на Windows CE 5.0 / MIPSII. Проєкт у розробці.

## Потрібна допомога з USB transport

Головне відсутнє підключення — реальний HOST EP0 provider для **Jungo USBware HOST 3.5.14.64**, платформа **LGE_AU1350_YN**. Потрібні сумісний SDK/API або реалізація виклику через чинного USB-власника з підтвердженим життєвим циклом пристрою.

- [Точний технічний запит / Technical help request](docs/USB_HELP.md)
- [Новіші результати перевірки owner-entry (2026-11-20)](docs/OWNER_ENTRY_EVIDENCE_2026-11-20.md)
- [Вихідний код QUERY-компонента та відтворювані host-тести](examples/usb-query-boundary/)
- [Надати матеріали або технічну відповідь](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1)

Опублікований компонент перевіряє результат QUERY, покоління підключення та втрату ownership. Він не містить драйвера USBware і не є готовою програмою для магнітоли. Тести використовують імітацію USB; EXE для встановлення тут немає.

## Як допомогти

[Інструкція для учасників](CONTRIBUTING.md) містить формат технічної відповіді та команди перевірки прикладу. Поточна ціль — доступний застосунку виклик через OEM-власника, захист device/context і справжнє завершення передачі. Повторний пошук старого архіву не є поточним пріоритетом.

Приклад перевірено GCC: 46/46 host assertions. Фізичний USB transport і готовий CarPlay ще не підтверджені.

## English

AutoKitMIPS aims to connect Carlinkit / CarPlay to the LAN2100 head unit running Windows CE 5.0 / MIPSII.

We need a platform-matched **USBware HOST EP0 provider**. The published C++98 sample provides an application-side QUERY boundary and reproducible host tests; it does not implement the native USB transport. See the [technical request](docs/USB_HELP.md), the [newer owner-entry evidence note](docs/OWNER_ENTRY_EVIDENCE_2026-11-20.md), and the [sample](examples/usb-query-boundary/). Please contribute relevant API documentation, a minimal integration example, or a precise correction through [issue #1](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1).

Please identify the source, platform/version and redistribution permission of shared materials. Do not post confidential SDKs, credentials or firmware dumps.


## Development handoff and next steps

The current engineering status, accepted review boundaries and next implementation gates are consolidated here:

- [Development status (2026-11-23)](docs/DEVELOPMENT_STATUS_2026-11-23.md)
- [USBware/JIPOD owner findings](docs/USBWARE_OWNER_FINDINGS_2026-11-23.md)
- [Implementation roadmap](docs/IMPLEMENTATION_ROADMAP.md)
- [Architecture and lifetime contract](docs/ARCHITECTURE_AND_LIFETIME_CONTRACT.md)
- [Validation matrix and release gates](docs/VALIDATION_MATRIX.md)
- [Contributor evidence template](docs/CONTRIBUTOR_EVIDENCE_TEMPLATE.md)
- [Engineering decision log](docs/DECISION_LOG.md)
- [Repository workflow](docs/WORKING_WITH_REPOSITORY.md)

These documents describe the current blocker accurately: the native USBware EP0 provider and Production LAN2100 transport are not yet proven or implemented.
