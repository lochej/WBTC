#!/bin/bash

echo "Registering WBTC services and apps"

#WBTC_MGR dir
WBTC_MGR=/home/debian/wbtc_manager


echo "Compiling device tree overlay"

$WBTC_MGR/fpga_utils/make_dtbo $WBTC_MGR/fluxcapacitor/fpga_ws2812/fpga_overlay

echo "Copying fpga_firmware to /lib/firmware"

cp -v $WBTC_MGR/fluxcapacitor/fpga_ws2812/fpga_firmware.rbf /lib/firmware

echo "Changing executable permission for apps"

chmod +x $WBTC_MGR/fluxcapacitor/fluxcapacitord.py
chmod +x $WBTC_MGR/fluxcapacitor/fluxcapacitorctl.py
chmod +x $WBTC_MGR/fluxcapacitor/ws2812_fpga_daemon
chmod +x $WBTC_MGR/fluxcapacitor/fpga_ws2812/rgb_fluxcapacitor.py


echo "Creating symlinks for apps:"

#Control process added to path
ln -sv $WBTC_MGR/fluxcapacitor/fluxcapacitorctl.py /usr/local/bin/fluxcapacitor
ln -sv $WBTC_MGR/timecircuit/timecircuitctl.py /usr/local/bin/timecircuitctl

echo "Creating symlinks for daemons:"

#Systemd bindings
ln -sv $WBTC_MGR/timecircuit/timecircuit.service /lib/systemd/system
ln -sv $WBTC_MGR/fluxcapacitor/fluxcapacitor.service /lib/systemd/system
ln -sv $WBTC_MGR/GPIO/button_poweroff.service /lib/systemd/system
ln -sv $WBTC_MGR/fluxcapacitor/ws2812_fpga_daemon.service /lib/systemd/system

echo "Enabling systemd services"

systemctl daemon-reload
systemctl enable timecircuit.service
systemctl enable fluxcapacitor.service
systemctl enable button_poweroff.service
systemctl enable ws2812_fpga_daemon.service

echo "Disabling sudo passwd and tty for timedatectl and date"
#Add timedatectl and date command to the
SUDOERS_FILE=/etc/sudoers.d/timeadmin

echo "debian ALL=(root) NOPASSWD:/usr/bin/timedatectl" > $SUDOERS_FILE
echo "debian ALL=(root) NOPASSWD:/bin/date" >> $SUDOERS_FILE
echo "Defaults: debian !requiretty" >> $SUDOERS_FILE

echo "Setup done"