# Nova - The endless space

This is the SJTU CG final homework —— Nova.  



场景建模及渲染，场景添加光照，光照可交互控制 - 20'

粒子系统特效，运动物理仿真，实时切换模型 - 30'

粒子光照、纹理映射 - 20'

粒子系统交互控制 - 20'

作业报告 - 10'



### History

* 1.5 Start
  * working on skybox
    * network skybox need reverse direction
  * complete skybox and find proper pictures ( red galaxy )
  * tomorrow should do spaceship
* 1.6 Better
  * find a little bit hideous texture, but it work fine anyway
    * problem: load obj multiple times
  * finding a spaceship
    * problem: not realistic
  * add light source
    * problem: sun is dark 
    * fixed dark sun, todo: partical effect sun
  * add asteroids
    * add light
    * add movement
    * not quite efficient
    * 独显反而更卡？
  * todo: random generated spaceships and rocks
    * forget to add to vector so no difference is shown
    * still not showing, why?
      * for(:) isn't reference
      * glsl not compatible
      * random number generate not proper
    * working, but asteroids disapear
      * VAO conflict
* 1.7 detail

  * earth multi-texture done
  * 镜面反射 done
  * 圆形模型只读一次 done
  * 多个星球 大小数量先做好 done
  * code clean up done
  * 优化粒子系统 不删除 直接复用 done
  * star glow
    * frame buffer and gaussian kernel?
    * done! rim lighting!
  * 一键切换地球
  * 居然这个时候修复了c4d和assimp的兼容问题 ?????? 喵喵喵
* 1.8 perfection
  * 画了一个space robot
    * 有裂缝
    * really dark, because of no normal
    * fixed! auto generated normal
  * 亮边缘再亮一点 done
  * 一键切换粒子  done
  * 天空盒done
  * 换更好看的天空盒！github上找的
  * 太阳微调
  * 太阳shader failed
    * seems impossible to achieve partical effect in shader
* 1.9 final rush
  * sun brightness adjustment done
  * == instead of =
  * generateFire会闪屏 而且陨石不见了
    * vbo bind incorrectly
    * now make it billboard
    * switch partical doesn't work
      * myrobot wrong shader
    * earth wierd light
    * wierd white glitches
      * [!!!!]BufferData size shouldn't change
    * transpose not working
    * default angle can't be seen， so after rotate still can never be seen


  * spaceship tail partical effect
  * 彗星
  * spaceship action ( not important )
  * clean up function (public name use CAPITAL)
  * fade-in fade-out background-music
* 1.9 cleanup and document
* 1.10 documentation and improvement

### Achivement

- [x] 将partical要用的数据使用class放在一起，帧率翻倍
- [x] Instanciation
- [x] 地球4纹理，到晚上会开灯
- [x] 提高40%帧率 by less rotation(no rotation at all 90fps : all rotation 40fps)
- [x] 地球边缘大气层发光！
- [x] 自己画Model!


### Problem

* Low-poly model don't work well
  * collect relative technique in browser



### OpenGL main framework

* Vertices, Textures, shaders
* VAO, VBO
* set shader, draw
* core-profile



## Todo List
### 太阳系

- [x] 星球纹理(颜色，高光)
- [ ] 只给shader传时间，glsl使用两个极坐标运算位置(减少CPU->GPU)


- [x] 太阳：粒子特效发光
- [x] 恒星：实例化大量星环
- [x] 地球：超漂亮
- [x] 月球：更漂亮 微光
- [x] 飞行物：随机生成小陨石及无敌道具
- [x] 背景：天空盒，闪烁群星

### 飞船

* [x] 飞船基本实现

- [ ] 船尾火焰粒子效果
- [x] 超好看纹理，被月球和太阳照亮

### 技术

- [ ] 运动：变加速度运动而不是瞬时运动
- [x] 字体渲染
- [x] 使用独显(从NVIDIA文档里找来的)




##  History

- 鱼和海洋
- 樱花、银杏叶
- 宇宙与星环
- 音乐喷泉

core-profile mode(核心模式) instead of immediate mode(固定渲染管线)



Mipmap
=================================================================
问题：
[v] glew.h
	SysWOW64 instead of System32
[v] exit重定义
	include iostream before GL
[v] 键盘延迟
	GetKeyState()
[v] obj重定义
	call和定义的参数类型不同(隐式声明)
[v] gl卡死
	glewInit()!
[v] 不显示
	clearDepthBufferBit
[v] assimp读取材料色
	很辛苦的看document
[v] model load in error
	全部合并后导出obj
	或者.x格式！
	fbx better!
[v] singleton
	static both in .h and .cpp
[v] 光照纹理贴图
=================================================================
TODO:
支持滚轮
减少状态切换
load model miss 不报错
*****************************************************************/