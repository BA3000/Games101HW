## Homework 1 笔记

这次的作业要求是填写一个旋转矩阵和透视投影矩阵，理论上简单代入公式就可以，但是还是有一些细节需要注意，因此简单记录一下。

## 初始作业问题

> 本次作业的任务是填写一个旋转矩阵和一个透视投影矩阵。给定三维下三个点 v0(2.0, 0.0, −2.0), v1(0.0, 2.0, −2.0), v2(−2.0, 0.0, −2.0), 你需要将这三个点的坐标变换为屏幕坐标并在屏幕上绘制出对应的线框三角形 (在代码框架中，我们已经提供了 draw_triangle 函数，所以你只需要去构建变换矩阵即可)。简而言之，我们需要进行模型、视图、投影、视口等变换来将三角形显示在屏幕上。

## 程序相关

### 踩坑

完成附加部分的时候遇到了一个 Exception，当三角形旋转到一定角度，部分像素点出现在屏幕外的时候，就会报数组越界的错误。检查后发现是 `rasterizer.cpp` 的 `set_pixel` 函数出了问题，没有判定是否越界，加上判定就可以修复：

```C++
void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    if (point.x() < 0 || point.x() >= width ||
        point.y() < 0 || point.y() >= height) return;
    auto ind = (height-point.y())*width + point.x();

    // 修复的部分
    if (ind >= frame_buf.size()) {
        return;
    }

    frame_buf[ind] = color;
}
```
