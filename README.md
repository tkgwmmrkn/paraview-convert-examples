# Paraviewへのファイル形式への変換例

Paraviewは
* レガシーVTKファイルフォーマット (.vtk)
* XML式 VTKファイルフォーマット (.vtu, .vti, .vtp等)
* Paraview パーティクルファイルフォーマット (.particles)
などを読み込むことができる．

### 一覧
* 等間隔データ： [VTK Image File(.vti)作成](https://github.com/tkgwmmrkn/paraview-convert-examples/blob/main/toVTK.cpp)
* 点群データ： [Paraview Particle File(.particles)作成](https://github.com/tkgwmmrkn/paraview-convert-examples/blob/main/ohtake.cpp)
* 主応力ベクトル： [VTK Unstructured Grid(.vtu)作成](https://github.com/tkgwmmrkn/paraview-convert-examples/blob/main/principle_stress.cpp)

### 使い方等
* 点群からメッシュを作成する例 [Paraview 使い方 - 3次元主成分空間 - 2021 Oct 19](https://www.youtube.com/watch?v=H1SaZZKiJ1U)