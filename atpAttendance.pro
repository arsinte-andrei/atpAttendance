TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = usb pontaj

usb.file = usb/libQDeviceWatcher.pro
pontaj.file = pontaj/pontaj.pro
pontaj.depends += libqdevicewatcher
