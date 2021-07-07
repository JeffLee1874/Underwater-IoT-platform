# Underwater-IoT-platform
水下物联网平台

环境（Host和Vnode环境均一致）：
Ubuntu 20.04 LTS(内核版本5.8.0)

## Kernel 模块
**步骤为：启动 - 修改 - make - insmod，注意一定要先启动VM再加载模块，后续可以通过使用命令行方式来实现动态加载**
1. qemu启动虚拟机：`qemu-system-x86_64 -m 512 -hda XXX.img -net nic,macaddr=XX:XX:XX:XX:XX:XX -net tap --enable-kvm`.`--enable-kvm`需要支持kvm，如你的PC不支持，则去掉即可。虚拟节点镜像(img)文件在[https://pan.baidu.com/s/1OR3uVKWJa3fpF6UmxCZxrw], 提取码：`1234`
2. 修改Vnode注册信息：`hello.c`中`vnode_register(vn, "tap0", ps, vp)`需要修改，第二个参数为虚拟网卡后端名称。虚拟网卡后端名称通过在Host上`ifconfig`命令查看。`n`个Vnode则需要注册`n`次
3. 编译：`sudo make`
4. 加载模块：`insmod ABC.ko`

##  vmac 模块
1. 编译：`sudo make`
2. 加载模块：`insmod vmac_modules.ko`