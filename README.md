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



### Problem

* Low-poly model don't work well



### OpenGL main framework

* Vertices, Textures, shaders
* VAO, VBO
* set shader, draw



## Todo List
### 太阳系

* 总体：
  * 多种纹理(颜色，高光)
  * 只给shader传时间，glsl使用两个极坐标运算位置(减少CPU->GPU)


* 太阳：粒子特效发光
* 恒星：实例化大量星环
* 地球：超漂亮
* 月球：更漂亮 微光
* 飞行物：随机生成小陨石及无敌道具
* 背景：天空盒，闪烁群星

### 飞船

* 船尾火焰粒子效果
* 四元旋转(待定)
* 超好看纹理，被月球和太阳照亮
* 飞船前部点光源

### 技术

* 碰撞检测
  * 飞船最后渲染，此前使用深度buffer判断是否物体进入屏幕特定位置，是则撞毁
  * (persumably optimum solution in the world)
* 滤镜：撞毁后添加毁灭滤镜
* 运动：变加速度运动而不是瞬时运动
* 字体渲染
* 使用独显(从NVIDIA文档里找来的)




##  History

- 鱼和海洋
- 樱花、银杏叶
- 宇宙与星环
- 音乐喷泉

core-profile mode(核心模式) instead of immediate mode(固定渲染管线)