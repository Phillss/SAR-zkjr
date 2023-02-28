from PIL import Image
import numpy as np
import xml.dom.minidom
import os
import shutil
import random
import cv2
import tqdm
import xml.etree.ElementTree as ET


def setDir(file_path):
    if not os.path.exists(file_path):
        os.makedirs(file_path)
    else:
        shutil.rmtree(file_path)
        os.makedirs(file_path)

def get_fileNames(rootdir,file_type):
    fs=[]
    for file in os.listdir(rootdir):
        ending = os.path.splitext(file)[-1]
        if ending == file_type:
            fs.append(os.path.join(rootdir, file))
    return fs

def read_xml(path):
    # 读取xml中的目标信息
    tree = ET.parse(path)
    root = tree.getroot()
    objects = root.findall('object')
    obj_info = []  # 存xml中的目标信息
    for obj in objects:
        obj_name = obj.find('name').text
        obj_bbox_xmin = obj.find('bbox').find('xmin').text
        obj_bbox_xmax = obj.find('bbox').find('xmax').text
        obj_bbox_ymin = obj.find('bbox').find('ymin').text
        obj_bbox_ymax = obj.find('bbox').find('ymax').text
        obj_info.append([obj_name, int(obj_bbox_xmin), int(obj_bbox_ymin), int(obj_bbox_xmax), int(obj_bbox_ymax)])
    return obj_info

def compute_iou(rec1, rec2):
    """
    computing IoU
    :param rec1: (y0, x0, y1, x1), which reflects (top, left, bottom, right)
    :param rec2: (y0, x0, y1, x1)
    :return: scala value of IoU
    """
    # computing area of each rectangles
    S_rec1 = (rec1[2] - rec1[0]) * (rec1[3] - rec1[1])
    S_rec2 = (rec2[2] - rec2[0]) * (rec2[3] - rec2[1])
 
    # computing the sum_area
    sum_area = S_rec1 + S_rec2
 
    # find the each edge of intersect rectangle
    left_line = max(rec1[1], rec2[1])
    right_line = min(rec1[3], rec2[3])
    top_line = max(rec1[0], rec2[0])
    bottom_line = min(rec1[2], rec2[2])
 
    # judge if there is an intersect
    if left_line >= right_line or top_line >= bottom_line:
        return 0
    else:
        intersect = (right_line - left_line) * (bottom_line - top_line)
        return (intersect / (sum_area - intersect))*1.0

def sliceImg(image_list):
    # image_list: 待切片图片路径：[img1_path,img2_path,img3_path...]
    # 每张待切图片处理一次
    for i in range(len(image_list)):
    
        image_dir = image_list[i]
        print(i, image_dir)

        image = cv2.imread(image_dir)
        img_name = os.path.basename(image_dir).replace('.jpg','')
        image = np.array(image) # 转换为numpy来处理

        width_orig = image.shape[1] # 原始图像宽度
        height_orig = image.shape[0] # 原始图像高度

        # 原始图像 xml 信息
        xml_dir = os.path.join(OrigXmlPath,os.path.basename(image_dir).replace('.jpg','.xml'))
        anno_objs = read_xml(xml_dir)  # [[name, xmin, ymin, xmax, ymax],...], x为h方向
        if len(anno_objs) == 0:
            print('No objects in ', img_name)
            break
        
        imgCount = 0  # 切片 id
        for h in range(0, height_orig, overlap):
            for w in range(0, width_orig, overlap):
                
                imgCount += 1
                target_info = []  # 记录落在当前切片中的原图目标

                # 从原图切片，获取切片内容
                hmin, hmax, wmin, wmax = h, h+height_crop, w, w+width_crop  # 未超出边界
                if width_orig - w < width_crop:  # 仅 w 方向超出边界
                    hmin, hmax, wmin, wmax = h, h+height_crop, width_orig-width_crop, width_orig
                if height_orig - h < height_crop:  # 仅 h 方向超出边界
                    hmin, hmax, wmin, wmax = height_orig-height_crop, height_orig, w, w+width_crop
                if width_orig - w < width_crop and height_orig - h < height_crop:  # w,h 都超出边界
                    hmin, hmax, wmin, wmax = height_orig-height_crop, height_orig, width_orig-width_crop, width_orig

                image_crop = image[hmin:hmax, wmin:wmax].astype('uint8')

                # 遍历原图中所有目标，判断目标是否在当前切片中
                for obj_id in range(len(anno_objs)):
                    name, xmin, ymin, xmax, ymax = anno_objs[obj_id]
                    obj_area = (ymax - ymin) * (xmax - xmin)  # 目标的边界框面积

                    # 目标框全部在切片外
                    if compute_iou([xmin, ymin, xmax, ymax], [hmin, wmin, hmax, wmax]) == 0:
                        continue
                    # 目标框全部或部分在切片内
                    if compute_iou([xmin, ymin, xmax, ymax], [hmin, wmin, hmax, wmax]) > 0:
                        # 目标框全部在切片内
                        if wmin<ymax<wmax and wmin<ymin<wmax and hmin<xmax<hmax and hmin<xmin<hmax:
                            xmin_, ymin_, xmax_, ymax_ = xmin - hmin, ymin - wmin, xmax - hmin, ymax - wmin
                        # 仅 w 方向超出边界
                        if ymin < wmin:
                            xmin_, ymin_, xmax_, ymax_ = xmin - hmin, 0, xmax - hmin, ymax - wmin
                        if ymax > wmax:
                            xmin_, ymin_, xmax_, ymax_ = xmin - hmin, ymin - wmin, xmax - hmin, wmax - wmin
                        # 仅 h 方向超出边界
                        if xmin < hmin:
                            xmin_, ymin_, xmax_, ymax_ = 0, ymin - wmin, xmax - hmin, ymax - wmin
                        if xmax > hmax:
                            xmin_, ymin_, xmax_, ymax_ = xmin - hmin, ymin - wmin, hmax - hmin, ymax - wmin
                        # w,h 均超出边界
                        if ymin < wmin and xmin < hmin:  # 左上
                            xmin_, ymin_, xmax_, ymax_ = 0, 0, xmax - hmin, ymax - wmin
                        if ymax > wmax and xmax > hmax:  # 右下
                            xmin_, ymin_, xmax_, ymax_ = xmin - hmin, ymin - wmin, hmax - hmin, wmax - wmin
                        if ymax > wmax and xmin < hmin:  # 右上
                            xmin_, ymin_, xmax_, ymax_ = 0, ymin - wmin, xmax - hmin, wmax - wmin
                        if ymin < wmin and xmax > hmax:  # 左下
                            xmin_, ymin_, xmax_, ymax_ = xmin - hmin, 0, hmax - hmin, ymax - wmin


                    partial_obj_area = (ymax_ - ymin_) * (xmax_ - xmin_)  # 此时部分目标的边界框面积
                    if partial_obj_area / obj_area > iou:
                        if xmin_<0 or ymin_<0 or xmax_<0 or ymax_<0:
                            a = 1
                        target_info.append([name, xmin_, ymin_, xmax_, ymax_])
                
                image_name = img_name + '_' + '%d_%d'%(hmin, wmin) + '.jpg'
                image_output = Image.fromarray(image_crop)
                image_output.save(os.path.join(ImgOutputPath, image_name))

                # 写入 xml 文件
                root = ET.Element('annotation')  # 根节点

                filename = ET.SubElement(root, "filename")  # 一级子节点 - 图片名
                filename.text = image_name

                size = ET.SubElement(root, "size")  # 一级子节点 - 图片大小
                width = ET.SubElement(size, "width")  # 二级子节点
                height = ET.SubElement(size, "height")  # 二级子节点
                width.text = str(width_crop)
                height.text = str(height_crop)

                for k in range(len(target_info)):  # 目标框信息
                    target = target_info[k]  # (height, width)
                    
                    object = ET.SubElement(root, "object")  # 一级子节点

                    name = ET.SubElement(object, "name")    # 二级子节点
                    name.text = target[0]
                    
                    bbox = ET.SubElement(object, "bndbox")    # 二级子节点
                    xmin = ET.SubElement(bbox, "xmin")  # 三级子节点
                    ymin = ET.SubElement(bbox, "ymin")
                    xmax = ET.SubElement(bbox, "xmax")  # 此处 x 为 height 方向
                    ymax = ET.SubElement(bbox, "ymax")
                    xmin.text = str(target[1])
                    ymin.text = str(target[2])
                    xmax.text = str(target[3])
                    ymax.text = str(target[4])

                tree = ET.ElementTree(root)
                xml_name = XmlOutputPath + '/' + img_name + '_' + '%d_%d'%(hmin, wmin) + '.xml'
                tree.write(xml_name, encoding='utf-8', xml_declaration=True)
    return 


if __name__ == "__main__":
    
    # 目标切片大小
    width_crop = 256
    height_crop = 256

    # 滑窗间隔
    overlap = 100  

    # 目标在切片内出现多少才允许被保留
    iou=0.5  

    OrigImgPath =  r'1'      # 存放待切片图片
    ImgOutputPath = r'2'     # 存放切片后小图

    OrigXmlPath = r'1xml'    # 存放待切片图片标签
    XmlOutputPath = r'2xml'  # 存放切片后小图标签
    
    # 创建图像处理后保存的文件夹
    setDir(ImgOutputPath)
    setDir(XmlOutputPath)

    image_list = get_fileNames(OrigImgPath,'.jpg')
    print('image_num=',len(image_list), 'xml_num=',len(image_list))

    sliceImg(image_list)
