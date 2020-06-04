// OpenGL希望每次顶点着色器运行后
// 我们可见的顶点都为标准化设备坐标
// 也就是说每个顶点的x,y,z都应该在-1.0到1.0之间
// 超出这个坐标范围的顶点将不可见
// 我们通常先自己设定一个坐标的范围
// 之后再在顶点着色器中将这些坐标转换为标准化设备坐标
// 但后将这些标准化设备坐标传入光栅器 将他们变换为屏幕上的二维坐标或像素
//
// 将坐标转换为标准化设备坐标 再转换为屏幕坐标过程是分步进行的
// 物体的顶点在最终转为屏幕坐标之前还会被变换到多个坐标系统
// 将物体的坐标变换到几个过渡坐标系的优点
// 一些操作和运算更加容易方便
//
// 五个不同的重要坐标系统
// 局部空间Local Space(物体空间Object Space)
// 世界空间World Space
// 观察空间 View Space(视觉空间 Eye Space)
// 裁剪空间Clip Space
// 屏幕空间 Screen Space
//
//
// 为将一个坐标从一个坐标系变换到另一坐标系
// 需要几个变换矩阵
// 最重要的分别是 模型 Modle 观察 View 投影 Projection三个矩阵
// 顶点坐标起始于局部空间 这里称其为局部坐标
// 之后变换为世界坐标 观察坐标 裁剪坐标 最后以屏幕坐标的形式结束
//
// 局部坐标是对象相对于局部原点的坐标 ，也是物体起始的坐标
// 下一步是将局部坐标转换为世界空间坐标
// 世界空间坐标是处于一个更大的空间范围的
// 这些坐标相对于世界的全局原点 会和其他物体一起相对于世界的原点进行摆放
// 接下来将世界坐标转换为观察空间坐标
// 使得每个坐标都是从摄像机或者说观察者的角度进行观察的
// 坐标到达观察空间之后
// 我们需将其头投影到裁剪坐标
// 裁剪坐标会被处理至-1.0到1.0的范围内并判断哪些顶点会出现在屏幕上
// 最后将裁剪坐标变换为屏幕坐标 将使用视口变换 Viewport Transform
// 视口变换将位于-1.0到1.0范围的坐标变换到由glViewport函数所定义的坐标范围内
// 最后变换的坐标会送到光栅器 将其转换为片段
//
//
// 局部空间
// 局部空间是指物体所在的坐标空间 即对象最开始所在的地方
// 创建的模型的所有顶点都以(0,0)为原点，即使他们最终出现在世界的不同位置
// 之前使用的箱子的顶点被设定在-0.5到0.5的坐标范围中，(0,0)是他的原点
//
// 世界空间
// 如果将所有的物体导入到程序当中
// 他们可能会全部聚集在世界的原点(0,0,0)上
// 于是我们想为每一个物体定义一个位置，从而能在更大的世界当中放置他们
// 如果希望将物体分散在世界上摆放
// 物体的坐标将会从局部变换到世界空间
// 该变换是有模型矩阵Model Matrix实现的
// 模型矩阵是一种变换矩阵 通过对物体进行位移 缩放 旋转 
// 来将它置于他本应该在的位置或朝向
// 想象变换一个房子 需要先将其缩小(它在局部空间中太大了)
// 然后将其位移 旋转
//
//
// 观察空间
// 观察空间通常被称为摄像机
// 所以有时称为摄像机空间或视觉空间
// 观察空间是将世界空间坐标转换为用户视野前方的坐标而产生的结果
// 因此观察空间坐标就是从摄像机视角观察到的空间
// 这通常是由一系列的位移和旋转的组合来完成
// 这些组合在一起的变换 通常储存在一个观察矩阵中
// 它被用来将世界坐标变换到观察空间
//
// 裁剪空间
// 顶点着色器运行的最后 OpenGL期望所有的坐标都能落在一个特定的范围内
// 且任何这个范围外的点都应该被裁剪掉
//
// 因为将所有可见的坐标都指定在-1.0到1.0范围内不是很直观
// 所以我们会指定自己的坐标集 并将它变换回标准化设备坐标系
//
// 为了将顶点坐标从观察变换到裁剪空间
// 我们需要定义一个投影矩阵
// 它指定了一个范围的坐标
// 比如每个维度上的-1000到1000
// 投影矩阵会将这个指定的范围内的坐标变换为标准化设备坐标的范围(-1.0, 1.0)
// 所有在范围外的坐标不会被映射到-1.0到1.0的范围之间
//
// 如果只是图元 例如三角形的一部分超出了裁剪体积
// 则OpenGL会重新构建这个三角形为一个或多个三角形让其能够适合这个裁剪范围
// 由投影矩阵创建的观察箱被成为平截头体
// 每个出现在平截头体范围内的坐标 都会最终出现在用户的屏幕上
// 将特定范围内的坐标转换到标准化设备坐标系的过程被称为投影
// 因为使用3D坐标投影Project很容易映射到2D的标准化设备坐标系中
//
// 一旦所有的顶点被变换到裁剪空间
// 最终的操作——透视除法 Perspective Division
// 这个过程将位置向量的xyz分别除以向量的齐次w分量
// 透视除法是将4D裁剪空间坐标 变换为3D标准化设备坐标的过程
// 这一步会在每个顶点着色器运行的最后被自动执行
// 这一阶段后最终的最表会被映射到屏幕空间中(使用glViewport中的设定) 并被变换为片段
//
// 将观察坐标变换为裁剪坐标的投影矩阵可以为两种不同的形式
// 每种形式都定义了不同的平截头体
// 可以选择创建一个正射投影矩阵或一个透视投影矩阵
//
// 正射投影
// 定义了一个类似立方体的平截头箱
// 创建一个正射投影矩阵需要指定可见平截头体的长宽高
// 平截头体定义了可见的坐标 它由宽、高、近(Near)平面和远(Far)平面所指定
// 任何出现在近平面前或远平面后的坐标都会被裁剪掉
// 正射平截头体直接将平截头体内部的所有坐标映射为标准化设备坐标，因为每个向量的w分量都没有进行改变；如果w分量等于1.0，透视除法则不会改变这个坐标。
// 要创建一个正射投影矩阵 可以使用GLM的内置函数glm::ortho
// glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
// 前两个参数指定了平截头体的左右坐标
// 第三四参数指定了平截头体的底部和顶部
// 通过四个参数定义了近平面和远平面的大小
// 第五六参数定义了近平面和远平面的距离
// 正射投影矩阵会直接将坐标映射到2D平面
// 但实际上一个直接的投影矩阵会产生不真实的结果，因为这个投影没有将透视考虑进去，所以我们需要透视投影矩阵
//
// 透视投影
// 由于透视，距离越远的物体看起来更小
// 使用透视投影矩阵，
// 这个投影矩阵将给定的平截头体范围映射到裁剪空间
// 除此之外还修改了每个定点坐标的w值，从而使得离观察者越远的顶点坐标w分量越大
// 被变换到裁剪空间的坐标都会在-w到w的范围之间
// 一旦坐标落在-1.0到1.0的裁剪空间内后，透视除法就会被应用到裁剪空间的坐标上
// 顶点坐标的每个分量都会除以它的w分量
// GLM可以这样创建一个透视投影矩阵
// glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
// 同样它所做其实是创建了一个定义了可视空间的大平截头体
// 任何在这个平截头体以外的东西最后都不会出现在裁剪空间体积内
// 一个透视平截头体可以被看作一个不均匀的箱子
// 箱子内的每个坐标都会被映射到裁剪空间上的一个点
//
// 第一个参数是这了fov的值 表示视野(Field of View)
// 并且设置了观察空间的大小
// 如果想要一个真实的观察效果
// 它的值通常设置为45.0f
// 但想要一个末日风格的结果可以将其设置为更大的值
// 第二个参数设置了宽高比，由视口的宽除以高所得
// 第三和第四个参数设置了平截头体的近和远平面
// 通常设置近距离为0.1f而远距离为100.0f
//
// 当使用正射投影时，每一个顶点坐标都会直接映射到裁剪空间中而不经过任何精细的透视除法(它仍然会进行透视除法，只是w分量没有被改变，保持为1)
// 
