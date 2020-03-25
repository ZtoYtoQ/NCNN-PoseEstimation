# NCNN_SinglePoseEstimation
## Introduction 
The project mainly provides  pre-training model (trained from MPII dataset by Pytorch) to estimate the individual human posture through NCNN.(only  test in  Linux x86)

## Installation
the related  package:
1. g++ cmake Opencv (C++) 
2. [Protobuf](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md)  (Optional )
3. download model from [BaiduCloud](https://pan.baidu.com/s/1Nw24WJj2SjG0iANua4XIoA) (password：yp19) 

```bash
git clone https://github.com/ZtoYtoQ/NCNN_SinglePoseEstimation.git

cd {your clone root-dir}
mkdir build
cd build
cmake ..
make -j2
```
 ## Usage
```bash
cd ..   ## return  your clone root-dir
cp demo.jpg build/examples/
cp {your model download path /hg_pose.bin}  build/examples/
cp {your model download path /hg_pose.param}  build/examples/
cd build/examples 
./hgpose demo.jpg
```
## Task Lists
- [ ] Providing  training code
- [ ] Training  smaller model  
- [ ] building  on other platforms(Jetson、Raspberry...)
- [ ] More....

## Reference
NCNN：https://github.com/Tencent/ncnn

## License
[MIT](https://choosealicense.com/licenses/mit/)
