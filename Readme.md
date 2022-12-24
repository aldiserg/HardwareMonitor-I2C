# HardwareMonitor Arduino + python
Analog of https://github.com/AlexGyver/PCdisplay for linux OS. Just displayed system perfomance info 

![How it looks like](https://github.com/aldiserg/HardwareMonitor-I2C/blob/main/view.png?raw=true)
![Connection](https://github.com/aldiserg/HardwareMonitor-I2C/blob/main/connections.png?raw=true)

# Requirements
python3 and pip3

# Setup via script
Using install.sh

```
Usage:
./install.sh amd/nvidia gpuMemTotalMb /path/to/USB

Example:
./install.sh amd 12000 /dev/ttyUSB0
```
# Setup via shell
Upload sketch to arduino via arduino ide

Install libs + add access:

For NVIDIA gpu
```
pip3 install pyserial psutil nvitop
```

For AMD gpu
```
pip3 install pyserial psutil pyamdgpuinfo
```

Add permistion to /dev/ttyUSBx
```
sudo gpasswd -a username tty
sudo gpasswd -a username uucp

or

sudo chown username:usergroup /dev/ttyUSBx
```
Check tty device path
```
ls -l /dev/ttyUSB*
```
Run for check allright
```
python3 hwm.py
```

Create /etc/systemd/system/hwm.service file with following content
```
[Unit]
Description=Hardware Monitor
After=multi-user.target

[Service]
Type=simple
ExecStart=python /path/to/hwm.py
Restart=always

[Install]
WantedBy=multi-user.target
```

Add deamon to startup and run
```
systemctl enable hwm --now
```
