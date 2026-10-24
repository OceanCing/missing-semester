# 在终端执行，确保 DISPLAY=:0
export DISPLAY=:0
xhost +

sudo /opt/arcs/0.32.2-rc.5/bin/aubo_scope &
sudo /opt/arcs/0.32.2-rc.5/bin/aubo_control &

vscode编辑arcs的lua
```
sudo chown -R $USER:$USER /root/arcs_ws/program
```

sudo code /root/arcs_ws/program --user-data-dir=/tmp/root-vscode --no-sandbox



实际逻辑state = 
0 : 等待位
1：物块拍照点
11：等坐标
12：夹住物块
2：托盘拍照点
21：等坐标
3：任务书拍照点

python脚本仅作为发送格式参考：
#!/usr/bin/env python3

code /root/arcs_ws/program --no-sandbox --user-data-dir=/root/.vscode-root


sudo mount /dev/sdb1 /mnt/usb

sudo cp -r /root/arcs_ws/program /mnt/usb/