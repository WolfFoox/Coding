import h5py
import numpy as np
import cv2
import os

# 示例：读取图像文件
image_paths = ['./data/canong3_canonxt_sub_09.tif', './data/canong3_canonxt_sub_22.tif', './data/canong3_canonxt_sub_28.tif','./data/canong3_kodakdcs330_sub_04.tif','./data/canong3_kodakdcs330_sub_08.tif','./data/canong3_kodakdcs330_sub_17.tif','./data/canong3_kodakdcs330_sub_19.tif','./data/canong3_kodakdcs330_sub_26.tif','./data/canong3_kodakdcs330_sub_30.tif','./data/canong3_nikond70_sub_04.tif','./data/canong3_nikond70_sub_06.tif','./data/canong3_nikond70_sub_11.tif','./data/canong3_nikond70_sub_12.tif','./data/canong3_nikond70_sub_27.tif','./data/canonxt_kodakdcs330_sub_08.tif','./data/canonxt_kodakdcs330_sub_21.tif','./data/nikond70_canonxt_sub_01.tif','./data/nikond70_canonxt_sub_14.tif','./data/nikond70_canonxt_sub_15.tif','./data/nikond70_kodakdcs330_sub_12.tif','./data/nikond70_kodakdcs330_sub_16.tif']
images = [cv2.imread(path) for path in image_paths]
# images = [cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) for image in images]

# 统一图像大小（假设所有图像大小相同，可以根据实际情况调整）
width, height = 256, 256
images = [cv2.resize(image, (width, height)) for image in images]

# 转换图像数据类型为 float32，并确保维度一致
images = [image.astype(np.float32) for image in images]
images = [image if len(image.shape) == 3 else np.expand_dims(image, axis=-1) for image in images]

image_paths = ['./data/canong3_canonxt_sub_09_edgemask.jpg', './data/canong3_canonxt_sub_22_edgemask.jpg', './data/canong3_canonxt_sub_28_edgemask.jpg','./data/canong3_kodakdcs330_sub_04_edgemask.jpg','./data/canong3_kodakdcs330_sub_08_edgemask.jpg','./data/canong3_kodakdcs330_sub_17_edgemask.jpg','./data/canong3_kodakdcs330_sub_19_edgemask.jpg','./data/canong3_kodakdcs330_sub_26_edgemask.jpg','./data/canong3_kodakdcs330_sub_30_edgemask.jpg','./data/canong3_nikond70_sub_04_edgemask.jpg','./data/canong3_nikond70_sub_06_edgemask.jpg','./data/canong3_nikond70_sub_11_edgemask.jpg','./data/canong3_nikond70_sub_12_edgemask.jpg','./data/canong3_nikond70_sub_27_edgemask.jpg','./data/canonxt_kodakdcs330_sub_08_edgemask.jpg','./data/canonxt_kodakdcs330_sub_21_edgemask.jpg','./data/nikond70_canonxt_sub_01_edgemask.jpg','./data/nikond70_canonxt_sub_14_edgemask.jpg','./data/nikond70_canonxt_sub_15_edgemask.jpg','./data/nikond70_kodakdcs330_sub_12_edgemask.jpg','./data/nikond70_kodakdcs330_sub_16_edgemask.jpg']
images2 = [cv2.imread(path) for path in image_paths]

# 统一图像大小（假设所有图像大小相同，可以根据实际情况调整）
width, height = 256, 256
images2 = [cv2.resize(image, (width, height)) for image in images2]

# 转换图像数据类型为 float32，并确保维度一致
images2 = [image.astype(np.float32) for image in images2]
images2 = [image if len(image.shape) == 3 else np.expand_dims(image, axis=-1) for image in images2]



# 将图像数据转换为 NumPy 数组，并确保数据类型为 float32
images_np2 = np.array(images2, dtype=np.float32)
images_np2 = np.mean(images_np2, axis=-1)
# images_np2 = np.array(images_np2).reshape(-1, 3)



# 指定文件夹路径和文件名
folder_path = 'data_folder'
file_name = 'e2.hdf5'
file_path = os.path.join(folder_path, file_name)

# 确保文件夹存在，如果不存在则创建
if not os.path.exists(folder_path):
    os.makedirs(folder_path)

# 将图像数据转换为 NumPy 数组，并确保数据类型为 float32
images_np = np.array(images, dtype=np.float32)

# 创建 HDF5 文件
with h5py.File(file_path, 'w') as hdf_file:
    # 创建名为 'images' 的数据集，并将数据写入其中
    hdf_file.create_dataset('test_img', data=images_np)
    hdf_file.create_dataset('test_labels', data=images_np2)

    # 添加元数据（可选），使用正确的形状
    hdf_file.attrs['description'] = 'Example dataset with images'
    hdf_file.attrs['data_shape'] = images_np.shape

print(f'HDF5 file "{file_path}" created and saved successfully.')


