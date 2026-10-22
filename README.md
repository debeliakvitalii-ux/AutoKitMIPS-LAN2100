# AutoKitMIPS — LAN2100

Автономне підключення Carlinkit / CarPlay до магнітоли LAN2100 на Windows CE 5.0 / MIPSII. Проєкт у розробці.

## Потрібна допомога з USB transport

Головне відсутнє підключення — реальний HOST EP0 provider для **Jungo USBware HOST 3.5.14.64**, платформа **LGE_AU1350_YN**. Потрібні сумісний SDK/API або реалізація виклику через чинного USB-власника з підтвердженим життєвим циклом пристрою.

- [Точний технічний запит / Technical help request](docs/USB_HELP.md)
- [Вихідний код QUERY-компонента та відтворювані host-тести](examples/usb-query-boundary/)
- [Надати матеріали або технічну відповідь](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1)

Опублікований компонент перевіряє результат QUERY, покоління підключення та втрату ownership. Він не містить драйвера USBware і не є готовою програмою для магнітоли. Тести використовують імітацію USB; EXE для встановлення тут немає.

## English

AutoKitMIPS aims to connect Carlinkit / CarPlay to the LAN2100 head unit running Windows CE 5.0 / MIPSII.

We need a platform-matched **USBware HOST EP0 provider**. The published C++98 sample provides an application-side QUERY boundary and reproducible host tests; it does not implement the native USB transport. Please start with the [technical request](docs/USB_HELP.md), inspect the [sample](examples/usb-query-boundary/), and contribute relevant API documentation, a minimal integration example, or a precise correction through [issue #1](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1).

Please identify the source, platform/version and redistribution permission of shared materials. Do not post confidential SDKs, credentials or firmware dumps.
