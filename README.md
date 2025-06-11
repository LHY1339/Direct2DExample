# 课程大纲
## 1.创建窗口
- 注册窗口类
- 创建窗口并显示
- 消息循环

## 2.初始化D2D
- 创建 ID2D1Factory
- 创建 ID2D1HwndRenderTarget
- 设置背景为蓝色

## 3.绘制纯色矩形
- 创建 ID2D1SolidColorBrush
- 创建 D2D1_ROUNDED_RECT 设置绘制范围
- 绘制图形

## 4.绘制渐变矩形
- 创建 D2D1_GRADIENT_STOP 数组
- 创建 GradientStopCollection 并且绑定到 D2D1_GRADIENT_STOP 数组
- 创建 ID2D1LinearGradientBrush 并且绑定 GradientStopCollection
- 创建 D2D1_ROUNDED_RECT 设置绘制范围
- 绘制图形

## 5.绘制文字
- 创建 IDWriteFactory
- 创建 ID2D1SolidColorBrush 或者 ID2D1LinearGradientBrush
- 创建 IDWriteTextFormat
- 创建 D2D1_RECT_F 设置绘制范围
- 绘制文字

## 6.绘制图片
- 创建 IWICImagingFactory
- 编写 LoadBitmapFromFile 函数
- 创建 ID2D1BitmapBrush 
- 设置 ID2D1BitmapBrush 绘制参数
- 创建 D2D1_ROUNDED_RECT 设置绘制范围
- 绘制图片
