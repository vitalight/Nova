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
  * earth multi-texture
    * done
  * 镜面反射
    * done

  - star glow
    - frame buffer and gaussian kernel?

  * todo: sun partical effect
  * 多个星球 大小数量先做好
  * billboard
  * 圆形模型只读一次


### Achivement

* 将partical要用的数据使用class放在一起，帧率翻倍
* Instanciation
* 地球4纹理，到晚上会开灯


### Problem

* Low-poly model don't work well
  * collect relative technique in browser



### OpenGL main framework

* Vertices, Textures, shaders
* VAO, VBO
* set shader, draw



## Todo List
### 太阳系

- [ ] 星球纹理(颜色，高光)
- [ ] 只给shader传时间，glsl使用两个极坐标运算位置(减少CPU->GPU)


- [ ] 太阳：粒子特效发光
- [ ] 恒星：实例化大量星环
- [ ] 地球：超漂亮
- [ ] 月球：更漂亮 微光
- [x] 飞行物：随机生成小陨石及无敌道具
- [x] 背景：天空盒，闪烁群星

### 飞船

* [x] 飞船基本实现

- [ ] 船尾火焰粒子效果
- [ ] 四元旋转(待定)
- [ ] 超好看纹理，被月球和太阳照亮
- [ ] 飞船前部点光源

### 技术

- [ ] 碰撞检测
  * 飞船最后渲染，此前使用深度buffer判断是否物体进入屏幕特定位置，是则撞毁
  * (persumably optimum solution in the world)
- [ ] 滤镜：撞毁后添加毁灭滤镜
- [ ] 运动：变加速度运动而不是瞬时运动
- [x] 字体渲染
- [x] 使用独显(从NVIDIA文档里找来的)




##  History

- 鱼和海洋
- 樱花、银杏叶
- 宇宙与星环
- 音乐喷泉

core-profile mode(核心模式) instead of immediate mode(固定渲染管线)