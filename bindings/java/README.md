# Java Binding for libvmcu

## Disclaimer
This binding is still unfinished but it supports basic library calls for 
vmcu_system_t and vmcu_report_t. It is not yet fully tested.

## Supported libvmcu Features
- basic System Support (stepping, reading gprs, ...)
- basic Report Support (accessing all report members)

## Example

BasicExample.java provides a starting point.
You will have to adjust the Makefile to fit your Java Version.

```console
cd libvmcu-Virtual-MCU-Library
make
cd bindings/java
sudo make
cd driver/basic
make
./run.sh
```

## Requirements

- This binding works with libvmcu v.0.7.2

## Other

This Java Binding is currently maintained by pointbazaar.
