# QuickNTP

---

### Update your Nintendo Switch clock using a list of public NTP servers

Using a custom DNS (or a limited connection) it's not possible to update your clock with Nintendo's servers and the Switch appears to gain some minutes each month.

With this [Tesla](https://github.com/WerWolv/libtesla) plugin you can now quickly update your clock connecting to a public [NTP](https://en.wikipedia.org/wiki/Network_Time_Protocol) server!

[![QuickNTP badge][version-badge]][changelog] [![GPLv2 License Badge][license-badge]][license]

---

![Preview](https://user-images.githubusercontent.com/389887/191822037-254e038d-a878-4ab1-9e3d-d98f00051974.png)

## Features

- Update the time by selecting from a list of servers
- Show the current offset against the selected server
- Set the internal network time to the time set by the user in system settings (time traveling, yay!)

## Possible future features

- Better error handling / messages
- Show a clock with seconds
- Turn the heart icon in a clock (have to check for the font)
- Get the time in a separated thread
- Pick the closest NTP server based on user region
- Update the time with milliseconds (maybe a system limitation)

## Contributors

- [@DarkMatterCore](https://github.com/DarkMatterCore) (library updates)
- [@DraconicNEO](https://github.com/DraconicNEO) (new NTP servers)

## Credits

### Code and libraries

- [@3096](https://github.com/3096) for `SwitchTime` who gave me the initial idea and some code examples
- [@thedax](https://github.com/thedax) for `NX-ntpc`, used by [@3096](https://github.com/3096)
- [@SanketDG](https://github.com/SanketDG) for the [NTP Client](https://github.com/SanketDG/c-projects/blob/master/ntp-client.c) using `getaddrinfo` instead of `gethostbyname`
- [@WerWolv](https://github.com/WerWolv) for `libtesla`

### NTP Public servers

- [NICT公開NTPサービス](https://jjy.nict.go.jp/tsp/PubNtp/index.html)
- [インターネットマルチフィード(MFEED) 時刻情報提供サービス for Public](https://www.mfeed.ad.jp/ntp/)
- [Cloudflare Time Services](https://www.cloudflare.com/time/)
- [Google Public NTP](https://developers.google.com/time)

## Troubleshooting

- The "Synchronize Clock via Internet" option should be enabled in System settings [as described here](https://en-americas-support.nintendo.com/app/answers/detail/a_id/22557/p/989/c/188), since this program changes the "Network clock" (immutable in settings).

## Disclaimer

- Please don't send a lot of requests. NTP servers should be requested in 36 hours intervals
- This program changes NetworkSystemClock, which may cause a desync between console and servers. Use at your own risk! It is recommended that you only use the changed clock while offline, and change it back as soon as you are connected (either manually or using ntp.org server.)

[version-badge]: https://img.shields.io/github/v/release/nedex/QuickNTP
[changelog]: ./CHANGELOG.md
[license-badge]: https://img.shields.io/github/license/nedex/QuickNTP
[license]: ./LICENSE
