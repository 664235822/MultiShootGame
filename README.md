# MultiShootGame
一款虚幻4制作的射击对战游戏

## 项目介绍
* 使用Adobe Mixamo人物和动画。
* 支持多种主武器和副武器切换，每个武器有相应的数值比如伤害、精准度、后坐力、射速等，副武器每把类型不同，有狙击枪、榴弹发射器、火箭筒、散弹枪，额外还有手枪、匕首和手榴弹可使用。
* 可更改游戏画质，沙漠小镇地图支持白天、黑夜和晴天、雨天切换。
* 默认第三人称，瞄准时切换到第一人称，显示当前武器和手臂模型，未瞄准和已瞄准时分别有主武器和手枪的不同瞄准偏移效果。
* 在不同的情况下有画面抖动的效果，如站立、行走、跑步、下蹲、受到伤害、在手榴弹范围内等幅度不同。
* 脚步会跟随地面环境自动调整高度，跳跃时前面有障碍物会进行攀爬。
* 子弹打到不同的物体有不同的粒子效果，如金属、木材、石头和角色，特别是打到角色会溅血以外血液还会染红地面，并且子弹会留下弹孔，换弹时弹夹会掉落地面。
* 游戏提供了单人模式和多人模式，单人模式是和机器人进行对战，杀掉更多的机器人以获得高分，多人模式提供了专用服务器，可加入并游玩。
* 游戏有计分板，可显示分数，击杀数和死亡数，多人模式可显示其他玩家的计分信息。

## 已知未完成的功能
* 日夜系统（白天黑夜、晴天雨天）未同步，显示的是客户端进入游戏场景后设置的当前日夜天气
* 多人在线模式敌方武器模型未实现同步，显示的是己方人物的武器模型，但是发射的子弹类型（子弹、手榴弹、火箭筒、散弹）同步是正常的

## 项目视频
* (https://www.bilibili.com/video/BV1jF411K79N)
* (https://www.bilibili.com/video/BV1rP411A7Wt)

## 项目截图
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/1.png">
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/2.png">
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/3.png">
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/4.png">
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/5.png">
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/6.png">
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/7.png">
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/8.png">
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/9.png">
<img src="https://github.com/664235822/MultiShootGame/blob/main/img/10.png">

## 引擎版本 
* 4.27

## 使用方式
* 如果查看项目源代码，请下载源码版虚幻4.27发布版zip文件和并使用git和git-lfs克隆该项目源代码，对虚幻引擎进行编译，再通过代码编辑器打开游戏项目
* 注意，不能使用Epic Games启动器下载的二进制版虚幻4.27打开项目，该版本无法编译服务器端
* 注意，不能使用更高版本的虚幻版本，如虚幻5，更高的版本会有兼容性问题，如没有根骨骼绑定带来的人物行走漂移问题等
* 注意，不能直接下载项目源代码zip包，这样会缺乏git-lfs包含的项目二进制文件
* 项目游玩，单人模式可只下载客户端可执行程序
* 项目游玩，多人模式需要下载服务器端和客户端可执行程序，服务器端运行bat批处理文件，客户端在线游戏输入服务器ip地址，端口不用输入，默认为7777，再点击开始游戏

## 版权声明
* 作者 664235822 SmallPig1997
* 开源许可证 GPL-3.0 License
* 仅可用于学习交流，严禁用于商业用途
