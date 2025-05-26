# OpenGL_Minecraft
用OpenGL写的"我的世界"小游戏

 - 你可以使用使用45种不用样式的方块；
 - 你有机率出生在雪地中；
 - 你会看到动态的湖面波纹，在雪地中湖面会结冰；

## 1. 运行方式
双击运行 **OpenGL_Minecraft/OpenGL_MC/OpenGL_MC.exe** 文件

## 2. 游戏控制
| 角色控制 |按键 |
|-----------------------|---------------|
| 移动 |**W A S D** |
| 跳跃|**空格** |
|切换方块 |**1 2 3 4 5 6 7 8 9** 或 **鼠标滚轮**|
|放置方块 |**单击鼠标左键**|
|消除方块 |**按住鼠标右键** (不同方块消除难度不同) |

| 游戏控制 |按键 |
|-----------------------|------------|
| 暂停游戏并释放鼠标 |**ESC 键 或 B 键** |
| 打开背包|**B 键** |
|关闭背包 |**B 键 或 ESC 键**|
## 3. 更换方块

 1.  **B 键** 打开背包;
 2. **按住鼠标左键** 选中想要使用的方块;
 3. **拖拽方块至背包中的装备栏** 
 4. 拖拽装备栏中的方块可以**调整顺序** **`(主屏幕装备栏将同步更新)`**
 
![更换方块](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E6%9B%B4%E6%8D%A2%E6%96%B9%E5%9D%97.png) 

## 4. 自定义设置
**按下 ESC 键 或 B 键** 释放鼠标以修改设置 :
 1. 更换**天空风格** (Choose SkyStyle);
 2. 调整**移动速度** (Move Speed);
 3. 修改**跳跃高度** (Jump Height);
 4. 改变掉落**碎片的存留时间** (Cube Lifetime)；

## 5. 有意思的特点
### 5.1. 来自自然界的光照

> **夜晚月亮的平行光照**
> 
![平行光照](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E5%B9%B3%E8%A1%8C%E5%85%89%E7%85%A7.png)
### 5.2. 碎屑效果以及掉落碎片

> 1. 摧毁方块会产生随机数量的 **该方块的小碎屑** (图中青箭头所指的 小碎屑)
>  2. 摧毁方块掉落 **该方块的掉落碎片** (图中红色箭头所指的 黄色模板小方块)
>  
![碎屑碎片](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E7%A2%8E%E5%B1%91%E7%A2%8E%E7%89%87.png)
### 5.3. 摧毁方块时有破坏程度显示
![破坏程度](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E7%A0%B4%E5%9D%8F%E7%A8%8B%E5%BA%A6.png)
### 5.4. 指示鼠标目标方块

> **红框 指示鼠标正指向的方块**

![红框指示](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E7%BA%A2%E6%A1%86%E6%8C%87%E7%A4%BA.png)
### 5.5. 手持方块在装备栏中用方框指示

> **图中青色箭头所指 装备栏方框**

![装备栏方框](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E8%A3%85%E5%A4%87%E6%A0%8F%E6%96%B9%E6%A1%86.png)
### 5.6. 随机的小植物

> **包括红蘑菇、普通蘑菇、小花以及不同种类的小草**
> 
![小植物](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E5%B0%8F%E6%A4%8D%E7%89%A9.png)
### 5.7. 湖面
![湖面](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E6%B9%96%E9%9D%A2.png)
> **1. 湖面上有动态波纹（如下面的动图）**
> 
> **2. 你会掉入水中**
> 
> **3. 你在水中移动速度会变得缓慢**
> 
![动态湖面](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E5%8A%A8%E6%80%81%E6%B9%96%E9%9D%A2.gif)
> **如果在雪地中，湖面会结冰**
> 
![冰面](https://github.com/SiZhe/readmeImage/blob/main/OpenGL_Minecraft/%E5%86%B0%E9%9D%A2.png)
> 
PS:你要把 OpenGL_Minecraft/OpenGL_MC中的 glfw3.dll 和 imgui.ini 保存在 cmake-build-debug 和 cmake-build-release 中才可顺利运行!