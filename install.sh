#!/bin/bash
dir=`pwd`
gpu=$1
gpuMem=$2
tty=`echo \"$3\" | sed -e 's|/|\\\/|g'`

if [ -z $1 ] || [ "$1" != "amd" ] && [ "$1" != "nvidia" ]
then
echo $1
	echo "Usage:
./install.sh amd/nvidia gpuMemTotalMb /dev/ttyUSB0

Example:
./install.sh amd 12000 /dev/ttyUSB0
"
fi

# install deps
if [ "$1" == "amd" ]
then 
	sudo pip3 install pyserial psutil pyamdgpuinfo
fi

if [ "$1" == "nvidia" ]
then 
	sudo pip3 install pyserial psutil nvitop
fi

# change hwm.py with new settings
sed -i "/maxGpuMem=/c\maxGpuMem=$gpuMem" $dir/$gpu/hwm.py
sed -i "/arduinoPort=/c\arduinoPort=$tty" $dir/$gpu/hwm.py

# create service for sending data
sudo bash -c "cat > /etc/systemd/system/hwm.service <<EOF

[Unit]
Description=Hardware Monitor
After=multi-user.target

[Service]
Type=simple
ExecStart=python $dir/$gpu/hwm.py
Restart=always

[Install]
WantedBy=multi-user.target
EOF"

systemctl enable hwm --now