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

每当一个 EActionType::GetOutput被调用时，一个二进制数据包会被写入stdout

第一位为 EPlayerType
数据需要完整（包括第一位）发送给对应Type的客户端

## 获取棋盘

- 1 char Player
- 1 char BoardDisplay
- 1 char num of Commands

- 4 char * n commands
  - commands are shuffled to prevent 
### for each command 
- 1 char type 
- 1 char player ID Owner
  - 0 - 2 piece type
  - 7 - 13 action type
- 2 char meta
- #### meta
  - #### Piece
    - char 1 :
      - On board = 0xAA
      - Off board = 0x00
    - char 2:
      - Invalid : 0xFF
      - OffBoard : 0xAP P = playerID
      - OnBoard : 0xXY, X = row, Y = col
  - #### FireWall
    - char 1: 
      - On Board : 0xAA
      - Not On Board : 0x00
    - char 2:
      - Not Valid : 0xFF
      - On Board Slot : 0xXY, X = row, Y = col
  - #### LineBoost
    - char 1: 
      - On Board : 0xAA
      - Not On Board : 0x00
    - char 2:
      - On Board Slot : 0xXY, X = row, Y = col
      - Not Valid : 0xFF
  - #### NotFound
    - Used: 0xAAAA
    - NotUsed: 0x0000
  - #### SandBox
    - Used:
      - char 1 : 0xAA
      - char 2 : Slot, 0xXY, X = row, Y = col
    - NotUsed: 0x0000
  - #### RabbitTrap
    - Used:
      - char 1 :
        - Is Real/ Not revealed 0xAA 
        - Is Fake 0xA0
      - char 2 : Slot, 0xXY, X = row, Y = col
    - NotUsed: 0x0000
  - #### ZeroDayAttack
    - Used: 0xAAAA
    - NotUsed: 0x0000
  - 