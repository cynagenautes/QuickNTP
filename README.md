# QuickNTP

Syncs the Nintendo Switch time with a list of NTP Servers.

![Preview](https://user-images.githubusercontent.com/389887/78499263-e9fcc500-774f-11ea-9392-60bd19d21ad8.jpg)

## Features

- Update the time by selecting from a list of servers
- Show the current offset against the selected server

## Motivation

Using a custom DNS it's not possible to sync the time with Nintendo's servers, and the Switch appears to gain some minutes each month.

## Possible future features

- Better error handling / messages
- Show a clock with seconds
- Turn the heart icon in a clock (have to check for the font)
- Get the time in a separated thread
- Pick the closest NTP server based on user region
- Update the time with milliseconds (maybe a system limitation)

## Credit

- [@3096](https://github.com/3096) for SwitchTime who gave me the initial idea and some code examples
- [@thedax](https://github.com/thedax) for NX-ntpc, used by [@3096]
- [@SanketDG](https://github.com/SanketDG) for the [NTP Client](https://github.com/SanketDG/c-projects/blob/master/ntp-client.c) using `getaddrinfo` instead of `gethostbyname`
- [@WerWolv](https://github.com/WerWolv) for libtesla
- [NTP Pool Project](https://www.ntppool.org)

## Disclaimer

- Please don't send a lot of requests. NTP servers should be requested in 36 hours intervals
- This program changes NetworkSystemClock, which may cause a desync between console and servers. Use at your own risk! It is recommended that you only use the changed clock while offline, and change it back as soon as you are connected (either manually or using ntp.org server.)
