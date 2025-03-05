# 雷net重做

## 简介
- 雷net重做版的游戏逻辑后端部分。
- 从stdin接受服务端处理过的指令，处理后在stdout输出客户端应得的信息。

### 这个程序里包括的：
- 雷net的游戏逻辑
- 反作弊
- 装饰性指令同步
- 游戏内容同步
- 棋谱保存
- 玩家管理

### 这个程序里不包括的：
- 美术
- session管理
- websocket接收发送
- 账号管理
- 房间管理
- web数据初处理

## 创建


## 指令

玩家ID (1 char) + 指令头 (1 char) + 必备数据 (4 char) + （可选数据）

### 指令头

- 1 char
enum EActionType

### 其他

- #### 无meta
  - 
- #### 长数据
  - 1 uint32 (4 char) 可选数据长度 n
  - n char 数据

- #### InitializePiece
  - 由该玩家视角从左到右的棋子排列，1char

- #### 必备数据
  - 

## 输出


EPlayerType + Meta

### BD
- 每个棋子4 char，共4 * 8 * 8 char
- 表示方法后述
  
  
