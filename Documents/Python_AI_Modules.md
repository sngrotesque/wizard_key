1. Python核心库
### Numpy 库

官网：https://www.numpy.org.cn/

特点：
NumPy (Numerical Python) 是 Python 语言的一个扩展程序库，支持大量的维度数组与矩阵运算，此外也针对数组运算提供大量的数学函数库。

NumPy 通常 SciPy（Scientific Python）和 Matplotlib（绘图库）一起使用，这种组合广泛用于替代 MatLab，是一个强大的科学计算环境，有助于我们通过 Python 学习数据科学或者机器学习。

### SciPy 库

官网：https://www.scipy.org/

特点：
SciPy 是一个开源的 Python 算法库和数学工具包。SciPy 包含的模块有最优化、线性代数、积分、插值、特殊函数、快速傅里叶变换、信号处理和图像处理、常微分方程求解和其他科学与工程中常用的计算。它用于有效计算 Numpy 矩阵，使 Numpy 和 Scipy 协同工作，高效解决问题。

### Pandas 库

官网：https://pandas.pydata.org/

特点：
Pandas 是 Python 语言的一个扩展程序库，用于数据分析。Pandas 是一个开放源码、BSD许可的库，提供高性能、易于使用的数据结构和数据分析工具，基础是 Numpy（提供高性能的矩阵运算），可以从各种文件格式比如CSV、JSON、SQL、Excel导入数据。Pandas 可以对各种数据进行运算操作，比如归并、再成形、选择，还有数据清洗和数据加工特征。Pandas 广泛应用在学术、金融、统计学等各个数据分析领域。

### statsmodels 库

官网：https://www.statsmodels.org/

特点：
statsmodels 是一个 Python 库，用于拟合多种统计模型，执行统计测试以及数据探索和可视化。statsmodels 包含更多的“经典”频率学派统计方法，而贝叶斯方法和机器学习模型可在其他库中找到。包含在 statsmodels 中的一些模型：线性模型，广义线性模型和鲁棒线性模型，线性混合效应模型，方差分析（ANOVA）方法，时间序列过程和状态空间模型，广义的矩量法。



2. Python机器学习
### Scikit-Learn 库

官网：https://scikit-learn.org.cn/

特点：
Scikit-learn（以前称为scikits.learn，也称为sklearn）是针对 Python 编程语言的免费软件机器学习库。它具有各种分类，回归和聚类算法，包括支持向量机，随机森林，梯度提升，k均值和DBSCAN，并且旨在与 Python 数值科学库 NumPy 和 SciPy 联合使用。

### XGBoost 库

官网：https://xgboost.ai/

特点：
XGBoost是一个优化的分布式梯度增强库，旨在实现高效，灵活和便携。它在 Boosting框架下实现机器学习算法。XGBoost提供并行树提升（也称为GBDT，GBM），可以快速准确地解决许多数据科学问题。相同的代码在主要的分布式环境（Hadoop，SGE，MPI）上运行，并且可以解决数十亿个示例之外的问题。

### LightGBM 库

官网：https://lightgbm.readthedocs.io

特点：
LightGBM（Light Gradient Boosting Machine） 是微软开源的一个实现 GBDT 算法的框架，支持高效率的并行训练。LightGBM 提出的主要原因是为了解决 GBDT 在海量数据遇到的问题，让 GBDT 可以更好更快地用于工业实践。其具有以下优点：更快的训练速度、更低的内存消耗、更好的准确率、分布式支持，可以快速处理海量数据。

### CatBoost 库

官网：https://catboost.ai/

特点：
CatBoost 是由 Yandex 的研究人员和工程师开发的基于梯度提升决策树的机器学习方法，现已开源。CatBoost 在 Yandex 公司内广泛使用，用于排列任务、预测和提出建议。CatBoost 是通用的，可应用于广泛的领域和各种各样的问题。

### Eli5 库

官网：https://eli5.readthedocs.io

特点：
ELI5 是一个 Python 库，允许使用统一API可视化地调试各种机器学习模型。它内置了对多个ML框架的支持，并提供了一种解释黑盒模型的方法。它有助于调试机器学习分类器并解释它们的预测。

### Theano 库

官网：https://pypi.org/project/Theano/

特点：
Theano 是一个 Python 库，专门用于定义、优化、求值数学表达式，效率高，适用于多维数组。特别适合做机器学习。一般来说，使用时需要安装 Python 和 Numpy 。

### PyBrain库

官网：https://github.com/pybrain/pybrain/

特点：
PyBrain的概念是将一系列的数据处理的算法封装到被称之为Module的模块中。一个最小的Module通常包含基于机器学习算法的可调整的参数集合。

### Shogun库

官网：https://github.com/shogun-toolbox/shogun

特点：
Shogun是一个开源机器学习库，它提供广泛的高效和统一的机器学习方法，如多种数据表示、算法类和通用工具的组合，用于快速原型设计数据管道。



3. Python深度学习
### Chainer库

官网：https://www.cnpython.com/pypi/chainerrl

特点：
ChainerCV是一个基于Chainer用于训练和运行计算机视觉任务的神经网络工具。它涵盖了计算机视觉模型的高质量实现,以及开展计算机视觉研究的必备工具集。

###  PyLearn2库

官网：http://github.com/lisa-lab/pylearn2

特点：
Pylearn2是一个基于Theano的机器学习库,它的大部分功能是基于Theano顶层实现的。这意味着用户可以用数学表达式去编写Pylearn2插件(新模型、算法等)。

### Hebel库

官网：https://www.oschina.net/p/hebel/

特点：
Hebel 是一个通过 PyCUDA 库使用 GPU CUDA 来加速建立神经网络的深度学习库。它实现了几类最重要的神经网络模型，提供各种激活函数和训练模型。

### Neurolab库

官网：https://pythonhosted.org/neurolab/

特点：
neurolab是一个简单而强大的Python神经网络库。包含基于神经网络、训练算法和灵活的框架来创建和探索其他神经网络类型。

### TensorFlow 库

官网：https://www.tensorflow.org/

特点：
TensorFlow 是一个基于数据流编程（dataflow programming）的符号数学系统，被广泛应用于各类机器学习（machine learning）算法的编程实现，其前身是谷歌的神经网络算法库 DistBelief 。Tensorflow 拥有多层级结构，可部署于各类服务器、PC终端和网页并支持GPU和TPU高性能数值计算，被广泛应用于谷歌内部的产品开发和各领域的科学研究。

### PyTorch 库

官网：https://pytorch.org/

特点：
PyTorch 是一个开源的 Python 机器学习库，基于 Torch，用于自然语言处理等应用程序。PyTorch 的前身是 Torch ，其底层和 Torch 框架一样，但是使用 Python 重新写了很多内容，不仅更加灵活，支持动态图，而且提供了 Python接口。它是由 Torch7 团队开发，是一个以 Python 优先的深度学习框架，不仅能够实现强大的GPU加速，同时还支持动态神经网络。PyTorch 既可以看作加入了GPU支持的 Numpy，同时也可以看成一个拥有自动求导功能的强大的深度神经网络。除了 Facebook 外，它已经被Twitter、CMU 和 Salesforce 等机构采用。

### Keras 库

官网：https://keras.io/zh/

特点：
Keras 是一个由 Python 编写的开源人工神经网络库，可以作为 Tensorflow、 Microsoft-CNTK 和 Theano 的高阶应用程序接口，进行深度学习模型的设计、调试、评估、应用和可视化。Keras 在代码结构上由面向对象方法编写，完全模块化并具有可扩展性。Keras 支持现代人工智能领域的主流算法，包括前馈结构和递归结构的神经网络，也可以通过封装参与构建统计学习模型。在硬件和开发环境方面，Keras 支持多操作系统下的多GPU并行计算，可以根据后台设置转化为 Tensorflow、Microsoft-CNTK 等系统下的组件。

### Caffe2 库

官网：http://caffe.berkeleyvision.org/

特点：Caffe是由Berkeley Vision and Learning Center(BVLC)建立的深度学习框架。它是模块化的，速度极快。



4. Python分布式深度学习库
### dist-Keras 库

官网：https://joerihermans.com/work/distributed-keras/

特点：
dist-Keras 是在 Apache Spark 和 Keras 之上构建的分布式深度学习框架，其重点是“最先进的”分布式优化算法。以易于实现新的分布式优化器的方式设计了框架，从而使人们能够专注于研究。支持多种分布式方法，例如但不限于使用数据并行方法训练合奏和模型。

### elephas 库

官网：https://pypi.org/project/elephas/

特点：
elephas 是一个把 Python 深度学习框架 Keras 衔接到 Spark 集群的第三方 python 包。

### Spark-Deep-Learning 库

官网：https://databricks.github.io/spark-deep-learning/

特点：
Spark-Deep-Learning 为使用 Apache Spark 的 Python 中可伸缩的深度学习提供了高级api。该库来自 Databricks ，并利用 Spark 实现了两个最强大的方面：本着 Spark 和 Spark MLlib 的精神，它提供了易于使用的API，能够在很少的代码行中进行深入学习；它使用 Spark 强大的分布式引擎来扩展对海量数据集的深度学习。

### Mxnet库

官网：https://pypi.org/project/mxnet/

特点：
MXNet 是一款设计为效率和灵活性的深度学习框架。它允许你混合符号编程和命令式编程,从而最大限度提高效率和生产力。

### Sklearn-theano库

官网：https://github.com/sklearn-theano/

特点：
sklearn-theano的功能所在。你不能用它从头到尾的训练一个模型，但它的神奇之处就是可以把网络作为特征提取器。



5. Python自然语言处理
### NLTK 库

官网：https://www.nltk.org/

特点：
NLTK（Natural Language Toolkit）自然语言处理工具包，是 NLP 研究领域常用的一个 Python 库，由宾夕法尼亚大学的 Steven Bird 和 Edward Loper 在 Python 的基础上开发的一个模块，至今已有超过十万行的代码。这是一个开源项目，包含数据集、 Python 模块、教程等。

### SpaCy 库

官网：https://spacy.io/

特点：
SpaCy 是一个 Python 和 CPython 的 NLP 自然语言文本处理库。SpaCy主要功能包括分词、词性标注、词干化、命名实体识别、名词短语提取等等。

### PKUSeg 库

官网：https://pypi.org/project/pkuseg/

特点：
PKUSeg-Python 是由北京大学语言计算与机器学习研究组研制推出的一个高准确度的中文分词工具包。PKUSeg-Python 简单易用，支持多领域分词，在不同领域的数据上都大幅提高了分词的准确率。

### Gensim 库

官网：https://radimrehurek.com/gensim/

特点：
Gensim 是一个相当专业的主题模型 Python 工具包。在文本处理中，比如商品评论挖掘，有时需要了解每个评论分别和商品的描述之间的相似度，以此衡量评论的客观性。评论和商品描述的相似度越高，说明评论的用语比较官方，不带太多感情色彩，比较注重描述商品的属性和特性，角度更客观。Gensim 就是 Python 里面计算文本相似度的程序包。

### CoreNLP 库

官网：https://stanfordnlp.github.io/CoreNLP/

特点：
Stanford CoreNLP 提供了一套人类语言技术工具。支持多种自然语言处理基本功能，Stanford CoreNLP 是它的一个 Python 接口。Stanford CoreNLP 主要功能包括分词、词性标注、命名实体识别、句法结构分析和依存分析等等。

### TextBlob 库

官网：https://pypi.org/project/textblob/

特点：
用于处理文本数据的Python库。它提供一个简单的API，可用于深入研究常见的NLP任务，如词性标注、名词短语提取、情感分析、文本翻译、分类等。

### Stanfordnlp 库

官网：https://github.com/stanfordnlp/stanfordnlp

特点：
Stanford NLP提供了一系列自然语言分析工具。它能够给出基本的词形、词性，并且能够标记句子的结构，语法形式和字词的依赖，指明那些名字指向同样的实体，指明情绪，提取发言中的开放关系等。



6. Python计算机视觉
### openCV 库

官网：https://opencv.org/

特点：
OpenCV 是一个基于BSD许可（开源）发行的跨平台计算机视觉和机器学习软件库，可以运行在 Linux、Windows、Android 和 MacOS 操作系统上。它轻量级而且高效——由一系列 C 函数和少量 C++ 类构成，同时提供了 Python、Ruby、MATLAB 等语言的接口，实现了图像处理和计算机视觉方面的很多通用算法。

### Scikit-Image 库

官网：https://scikit-image.org/

特点：
Scikit-Image 是图像处理算法的集合，采用 Python 语言编写。它实现了用于研究、教育和工业应用的算法和实用程序。它是一个相当简单和直接的库，即使对于 Python 生态系统的新手也是如此。

### Pillow/PIL 库

官网：https://pillow.readthedocs.io/en/stable

特点：
PIL（Python Imaging Library）已经是 Python 平台事实上的图像处理标准库了。PIL 功能非常强大，但API却非常简单易用。由于 PIL 仅支持到 Python2.7，加上年久失修，于是一群志愿者在 PIL 的基础上创建了兼容的版本，名字叫 Pillow，支持最新Python 3.x，又加入了许多新特性。

### SimpleCV 库

官网：http://simplecv.org/

特点：
SimpleCV 将很多强大的开源计算机视觉库包含在一个便捷的 Python 包中。使用 SimpleCV，你可以在统一的框架下使用高级算法，例如特征检测、滤波和模式识别。使用者不用清楚一些细节，比如图像比特深度、文件格式、颜色空间、缓冲区管理、特征值还有矩阵和图像的存储。

### Mahotas 库

官网：https://pypi.org/project/mahotas/0.99/

特点：
Mahotas 是一个 Python 的图像处理库，包含大量的图像处理算法，使用 C++ 实现的算法，处理性能相当好。

### ITK 库

官网：https://itk.org/

特点：
ITK（ Insight Segmentation and Registration Toolkit）是美国国家卫生院下属的国立医学图书馆开发的一款医学图像处理软件包，是一个开源的、跨平台的影像分析扩展软件工具。

### Pgmagick 库

官网：https://pythonhosted.org/pgmagick/index.html

特点：
Pgmagick 是 GraphicsMagick 库的一个基于 Python 的包装器。图像处理系统有时被称为图像处理的瑞士军刀。它提供了一个健壮而高效的工具和库集合，支持以88种主要格式（包括重要格式，如DPX、GIF、JPEG、JPEG-2000、PNG、PDF、PNM和TIFF）读取、写入和操作图像。

### Pycairo 库

官网：https://www.cairographics.org/pycairo/

特点：
pyCairo 是一个 Python 的优秀2D图形渲染库。

### Fastai库

官网：https://pypi.org/project/fastai/

特点：
计算机视觉、文本、表格数据、时间序列、协同过滤等常见深度学习应用提供单一一致界面的深度学习库。

### Imutils库

官网：https://pypi.org/project/imutils/

特点：
imutils是在OPenCV基础上的一个封装,达到更为简结的调用OPenCV接口的目的,它可以轻松的实现图像的平移,旋转,缩放,骨架化等一系列的操作。

### PyTorchCV库

官网：https://pytorch-cn.readthedocs.io/zh/latest/

特点：
TorchCV 支持图像分类、语义分割、目标检测、姿态检测、实例分割、生成对抗网络等任务中的多个常见模型。



7. Python生物/化学
### BioPython 库

官网：https://biopython-cn.readthedocs.io/

特点：
Biopython 项目是旨在减少计算生物学中代码重复的开源项目之一，由国际开发人员协会创建。它包含表示生物序列和序列注释的类，并且能够读取和写入各种文件格式（FASTA，FASTQ，GenBank 和 Clustal 等），支持以程序化方式访问生物信息的在线数据库（例如，NCBI）。独立的模块扩展了 Biopython 的序列比对，蛋白质结构，群体遗传学，系统发育，序列基序和机器学习等功能。

### DashBio 库

官网：http://dash.plot.ly/dash-bio

特点：
Dash Bio 是一个免费的开源 Python 库,用于生物信息学和药物开发应用。

### RDKit 库

官网：http://www.rdkit.org/

特点：
RDKit 是一个用于化学信息学的开源工具包，基于对化合物2D和3D分子操作，利用机器学习方法进行化合物描述符生成，fingerprint 生成，化合物结构相似性计算，2D和3D分子展示等。基于Python语言进行调取使用。