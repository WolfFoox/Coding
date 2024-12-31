import h5py

# 以只读方式打开 HDF5 文件
# file_path = './test_data/test_imgs_feat_v2.hdf5'
# file_path = './test_data/test_imgs_v2.hdf5'
file_path = './data_folder/e.hdf5'
with h5py.File(file_path, 'r') as hdf_file:
    # 查看文件中的所有顶级键
    top_level_keys = list(hdf_file.keys())
    print("Top-level keys:", top_level_keys)

    # 遍历文件中的每个数据集并显示其名称和形状
    for key in top_level_keys:
        dataset = hdf_file[key]
        print(f"\nDataset: {key}")
        print("Shape:", dataset.shape)
        print("Data:")
        print(dataset[:])  # 显示数据集的内容，可以根据需要选择显示部分数据
