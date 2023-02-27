import cv2
import matplotlib.pyplot as plt
import random
import os
import numpy as np
import math
import xml.etree.ElementTree as ET
import imageio
import sys


def write_xml(target_info, position_x, position_y, xmlname):
    # 写入 xml 文件
    root = ET.Element('annotation')  # 根节点

    filename = ET.SubElement(root, "filename")  # 一级子节点
    filename.text = "background1.jpg"

    size = ET.SubElement(root, "size")  # 一级子节点
    width = ET.SubElement(size, "width")  # 二级子节点
    width.text = "1472"
    height = ET.SubElement(size, "height")  # 二级子节点
    height.text = "1784"

    for k in range(len(target_info)):
        target = target_info[k]  # (height, width)
        object = ET.SubElement(root, "object")
        name = ET.SubElement(object, "name")
        name.text = target[0]
        bbox = ET.SubElement(object, "bbox")
        xmin = ET.SubElement(bbox, "xmin")
        ymin = ET.SubElement(bbox, "ymin")
        xmax = ET.SubElement(bbox, "xmax")
        ymax = ET.SubElement(bbox, "ymax")
        xmin.text = str(position_x[k])
        ymin.text = str(position_y[k])
        xmax.text = str(position_x[k] + int(target[1][0]))
        ymax.text = str(position_y[k] + int(target[1][1]))

    tree = ET.ElementTree(root)
    tree.write(xmlname+".xml", encoding='utf-8', xml_declaration=True)


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


def rotate(img, angle):
    H, W = img.shape

    anglePi = angle * math.pi / 180.0
    cosA = math.cos(anglePi)
    sinA = math.sin(anglePi)
    new_height = math.ceil(H * np.cos(anglePi) + W * np.sin(anglePi))
    new_width = math.ceil(W * np.cos(anglePi) + H * np.sin(anglePi))

    out = np.zeros((new_height+1, new_width+1), dtype=np.uint8)  # 必须是8 不然显示不出图像

    dx_back = 0.5 * W - 0.5 * new_width * cosA - 0.5 * new_height * sinA
    dy_back = 0.5 * H + 0.5 * new_width * sinA - 0.5 * new_height * cosA
    for y in range(new_height):
        for x in range(new_width):

            x0 =int( x * cosA + y * sinA + dx_back)
            y0 = int(y * cosA - x * sinA + dy_back)
            if 0 < x0 < W and 0 < y0 < H:  # 计算结果是这一范围内的x0，y0才是原始图像的坐标。
                out[y, x] = img[y0, x0]  #

    return out


def load_target(root):
    # 读取目标图像
    filepath = os.listdir(root)
    image_array = []  # 存目标图像
    target_info = []  # 存目标种类和目标图像长宽
    for i in range(len(filepath)):
        imgspath = os.path.join(root, filepath[i])
        imgs = os.listdir(imgspath)
        for img in imgs:
            imgpath = os.path.join(root, filepath[i], img)
            image = cv2.imread(imgpath, 0)  # 101x101
            # image = rotate(image, 30)  # 目标图像绕中心点旋转
            image_array.append(image)
            object_class = imgspath.split('/')[-1]
            object_size = image.shape  # (height, width)
            target_info.append([object_class, object_size])  # 存目标种类
    return image_array, target_info


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


def map_target(image_array, background_objects, imgname, background):
    # 用目标图像覆盖背景图像 - PS:待添加目标不能覆盖background原有目标。
    img_num = len(image_array)
    img_objs = background_objects.copy()
    position_x = []
    position_y = []
    # 每添加一个目标，需遍历一次 background 中所有目标
    for i in range(len(image_array)):
        flag = False  # 目标添加成功时，flag=True
        while not flag:
            (H, W) = image_array[i].shape  # (H, W) 对应 (x, y)
            x = random.sample(range(80, 1700), 1)[0]  # 尽量不在图像边缘添加目标
            y = random.sample(range(80, 1400), 1)[0]
            be_add = [x, y, x+H, y+W]
            count = 0
            # 需遍历 background 中所有目标
            for j in range(len(img_objs)):
                obj_name, xmin, ymin, xmax, ymax = img_objs[j]
                obj_bbox = [xmin, ymin, xmax, ymax]
                iou = compute_iou(be_add, obj_bbox)
                # 若待添加目标bbox与原图目标bbox大于0，则需重新选择待添加目标的位置
                if iou > 0:
                    break
                count = count + 1
            if count == len(img_objs):  # 说明该目标不与任何目标重叠
                flag = True
                position_x.append(x)
                position_y.append(y)
                img_objs.append(['current', x, y, x+H, y+W])  # 防止后续添加的目标与之前添加的目标重叠

    for k in range(img_num):
        image = image_array[k]
        lenX, lenY = image.shape
        background[position_x[k]:position_x[k]+lenX, position_y[k]:position_y[k]+lenY] = image
    # plt.figure(2)
    # plt.imshow(background, cmap=plt.cm.gray)

    # 对添加目标后的背景图像进行高斯平滑处理
    for k in range(img_num):
        image = image_array[k]
        img_blur = cv2.GaussianBlur(image, (5,5), 1)  # 高斯滤波
        # img_blur = cv2.blur(image, (6,6))  # 均值滤波
        # img_blur = cv2.medianBlur(image, 5)  # 中值滤波
        lenX, lenY = image.shape
        background[position_x[k]:position_x[k]+lenX, position_y[k]:position_y[k]+lenY] = img_blur
    imageio.imsave(dist+imgname+".jpg", background)
    # plt.figure(3)
    # plt.imshow(background, cmap=plt.cm.gray)
    # plt.show()
    return position_x, position_y


def add_objects(img, xml, objects, num):
    # img: 背景图像路径
    # xml：背景图像标签
    # objects： 待添加目标
    # num：生成num张随机添加目标后的图片
    for i in range(0, num):
        # 读取背景图像
        background = cv2.imread(img, 0)  # 1784x1472

        # 读取背景图像对应的 xml 文件中的目标信息
        background_objs = read_xml(xml)

        # 读取目标图像
        image_array, target_info = load_target(root=objects)  # target_info: [['2S1', (70, 52)], ['BDRM-2', (19, 42)], ...]

        # 用目标图像覆盖背景图像，并对添加目标后的背景图像进行高斯平滑处理。同时返回目标在图中的位置。
        position_x, position_y = map_target(image_array=image_array, background_objects=background_objs, imgname=str(i), background=background)

        # 将目标信息写入 xml 文件
        write_xml(target_info=target_info, position_x=position_x, position_y=position_y, xmlname=str(i))

if __name__ == "__main__":
    originimg=sys.argv[1]#quanlujing
    filename=sys.argv[2]#wenjianming
    path=sys.argv[3]#peizhiwenjianlujing
    dist=sys.argv[4]
    img=originimg
    # img = origin+r'background1.jpg'
    xml = path+filename+'.xml'
    objects = path+r'obj_data'
    num = 1
    add_objects(img, xml, objects, num)
    