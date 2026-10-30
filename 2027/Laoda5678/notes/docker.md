```bash
# ============================================
# Docker 基本命令速查表
# 用法: bash docker-cheatsheet.sh或docker [对象] [操作] [参数]
# 注意: 大部分命令需要 sudo 或将用户加入 docker 组
# ============================================

以nginx:latest为镜像名，mynginx为容器名举例

# ---------- 1. 镜像相关 ----------

# 拉取镜像（下载）
docker pull nginx:latest    # 冒号后面也可以接其他tag即版本号如1.25、alpine,就会从Docker Hub上下载相应版本到本地

# 列出本地镜像
docker images
docker image ls

# 查看镜像详细信息
docker inspect nginx:latest

# 查看镜像历史（分层）
docker history nginx:latest

# 删除镜像
docker rmi nginx:latest
docker image rm nginx:latest

# 清理无用镜像（悬空镜像）
docker image prune -f

# 从 Dockerfile 构建镜像（-t 打标签，. 表示当前目录上下文）
docker build -t myapp:1.0 .

# 保存镜像为 tar 文件 / 从 tar 加载
docker save -o myapp.tar myapp:1.0
docker load -i myapp.tar

# ---------- 2. 容器相关 ----------

# 运行容器（前台交互）
docker run -it ubuntu:latest /bin/bash

# 运行容器（后台，映射端口，命名，挂载卷，环境变量）
docker run -d \
  --name mynginx \
  -p 8080:80 \
  -v /host/data:/container/data \
  -e TZ=Asia/Shanghai \
  --restart unless-stopped \
  nginx:latest

我常用的：
docker run -it --name rm_ros_v1 \
  --gpus all \
  --net=host \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  -v $HOME/.Xauthority:/root/.Xauthority:rw \
  -v ~/Desktop/workspace:/workspace \
  rm_ros_noetic_saved:latest bash
_____

# 列出运行中的容器
docker ps

# 列出所有容器（含已停止，-a是all的意思）
docker ps -a

# 启动 / 停止 / 重启 / 暂停 / 恢复 容器
docker start mynginx
docker stop mynginx
docker restart mynginx
docker pause mynginx
docker unpause mynginx

# 删除容器（-f 强制删除运行中的）
docker rm mynginx
docker rm -f mynginx

# 清理所有已停止容器
docker container prune -f

# 进入运行中容器（推荐 exec）
docker exec -it mynginx /bin/bash
docker exec -it mynginx bash/sh          # 精简镜像可能没有 bash

# 查看容器日志（-f 实时跟踪，--tail 最后 N 行）
docker logs -f --tail 100 mynginx

# 查看容器详细信息 / 进程 / 端口
docker inspect mynginx
docker top mynginx
docker port mynginx

# 容器与宿主机之间复制文件
docker cp mynginx:/etc/nginx/nginx.conf ./nginx.conf
docker cp ./index.html mynginx:/usr/share/nginx/html/

# 查看容器资源占用（实时）
docker stats

# 查看容器内文件系统变更
docker diff mynginx

# 提交容器为镜像
docker commit mynginx mynginx:snapshot

# ---------- 3. 网络相关 ----------

# 列出网络
docker network ls

# 创建自定义桥接网络
docker network create mynet

# 运行容器并加入网络
docker run -d --name app1 --network mynet nginx:latest

# 查看网络详情
docker network inspect mynet

# 连接 / 断开容器与网络
docker network connect mynet mynginx
docker network disconnect mynet mynginx

# 删除网络
docker network rm mynet

# ---------- 4. 数据卷相关 ----------

# 列出卷
docker volume ls

# 创建 / 查看 / 删除卷
docker volume create myvol
docker volume inspect myvol
docker volume rm myvol

# 使用命名卷运行容器
docker run -d --name db -v myvol:/var/lib/mysql mysql:8

# 清理未使用卷
docker volume prune -f

# ---------- 5. 系统信息与清理 ----------

# 查看 Docker 版本与系统信息
docker version
docker info

# 查看磁盘占用
docker system df

# 一键清理（停止的容器、悬空镜像、未用网络、构建缓存；加 -a 连未用镜像/卷）
docker system prune -f
docker system prune -a -f --volumes

# ---------- 6. Docker Compose（v2 插件形式） ----------

# 启动服务（-d 后台）
docker compose up -d

# 查看服务状态 / 日志
docker compose ps
docker compose logs -f

# 停止并删除容器、网络
docker compose down

# 停止并删除容器、网络、卷、镜像
docker compose down -v --rmi all

# 重新构建并启动
docker compose up -d --build

# ---------- 7. 常用组合示例 ----------

# 查看所有容器 IP
docker ps -q | xargs -r docker inspect -f '{{.Name}} -> {{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}'

# 批量停止所有容器
docker ps -q | xargs -r docker stop

# 批量删除所有容器
docker ps -aq | xargs -r docker rm -f

# 进入最新创建的容器
docker exec -it "$(docker ps -lq)" /bin/bash

# 在宿主机上开启图形权限
xhost +local:docker 