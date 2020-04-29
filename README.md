# NCNN_SinglePoseEstimation
## Introduction 
The project mainly provides  pre-trained model (trained from MPII dataset by Pytorch) to estimate the individual human posture through NCNN.

## Installation
the related  package:
1. g++ Cmake Opencv (C++) 
2. [Protobuf](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md)  (Optional )
3. download model from [BaiduCloud](https://pan.baidu.com/s/1Nw24WJj2SjG0iANua4XIoA) (Password：yp19) (FP32) , [BaiduCloud](https://pan.baidu.com/s/1QOCGh99e_3jVZgH9XMdhlQ) (Password：22qe) (INT8)
4. download  INT8 quantization Dataset ( MPII val Dataset) from [BaiduCloud](https://pan.baidu.com/s/1tS0rcevP-Sx3M3y-XxouHQ) (Password：fbma) (Optional )

NOTE: Protobuf version must match Cmake version (In this project, Protobuf is 3.11.4 Cmake is 3.10.2)

```bash
git clone https://github.com/ZtoYtoQ/NCNN_SinglePoseEstimation.git

cd {your clone root-dir}
mkdir build
cd build
cmake ..
make -j2
```
 ## Run
```bash
cd ..   ## return  your clone root-dir
cp demo.jpg build/examples/
cp {your model download path /hg_pose.bin}  build/examples/
cp {your model download path /hg_pose.param}  build/examples/
cd build/examples 
./hgpose demo.jpg
```
## INT8 Quantization

```bash
cd {your root-dir}
cp {your model download path /hg_pose.bin}  build/tools/quantize/
cp {your model download path /hg_pose.param}  build/tools/quantize/
mv {your INT8 quantization Dataset path}  build/tools/quantize/
cd build/tools/quantize/
### Create model quantization table ###
./ncnn2table --param=hg_pose.param  --bin=hg_pose.bin --images=MPII_val_image/  --output=hg_pose.table  --thread=2 --mean=112,113,110 --norm=0.004,0.004,0.004

### Quantization mdoel ###
./ncnn2int8 hg_pose.param hg_pose.bin hg_pose-int8.param hg_pose-int8.bin hg_pose.table

### Get int8 model  hg_pose-int8.param hg_pose-int8.bin ###
```



## Task Lists

- [x] Adding INT8 Quantization、INT8 model 
- [x] building  on [Android](https://github.com/ZtoYtoQ/NCNN_Android_SinglePoseEstimation) 
- [ ] Providing  training code
- [ ] Training  smaller model  
- [ ] building  on other platforms(Jetson、Raspberry...)
- [ ] More....

## Reference
NCNN：https://github.com/Tencent/ncnn

## License
[MIT](https://choosealicense.com/licenses/mit/)
