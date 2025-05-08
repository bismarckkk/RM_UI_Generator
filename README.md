# RM UI Generator
### 可以做什么
* 支持在本地cmake工程中引入从`.rmui`文件生成ui代码，不用再去ui设计器上生成代码，再拷贝到本地然后编写cmake文件了
* 🏗 为ui设计器代码生成功能提供底层支持（正在实现！）
### 有何要求
* 你的项目必须使用cmake作为构建工具，并且cmake的版本要大于3.10
* 如果你的系统不是64位的windows或linux系统，将必须从源码编译生成器，编译生成器需要支持C++20标准的编译器
### 如何使用
1. 集成  
   注意：无论使用哪种集成方式，cmake代码都建议添加到交叉编译工具链设置之前，如果采用命令行参数的形式指定了交叉编译工具链，
   则还需要在集成前设置`HOST_C_COMPILER`、`HOST_CXX_COMPILER`两个变量为你的宿主机编译器路径  
   * 最简单的集成方式是将以下代码添加到你的cmake工程中  
     这种方式将在首次运行时自动引入最新版本的ui生成器，后续将固定使用该版本生成器  
     缺点是必须要具备稳定的Github网络连接
     ```cmake
     set(addUIPath "${CMAKE_CURRENT_BINARY_DIR}/addUI.cmake")
     file(DOWNLOAD
         https://github.com/bismarckkk/RM_UI_Generator/releases/latest/download/addUI.cmake
         ${addUIPath}
     )
     include(${addUIPath})
     ```
   * 或者你也可以在release中下载心仪版本的`addUI.cmake`文件到工程目录，然后在主CMakeLists.txt中`include`该文件即可
     这种方式可以在无网络连接的情况下正常工作（还需要额外设置）  
     如果你需要在无网络连接的情况下工作，你还需要下载一些额外文件  
     如果你的平台在预购建支持范围以内，则需要在release中下载对应的可执行文件；如果没有对应的预购建，则需要自行下载源码编译  
     然后将可执行文件重命名为`rm_ui_generator(.exe)`，并放置在cmake构建目录下，这样生成器就可以在无网络的情况下工作了  
   
2. 生成
    首先建议在集成的`include`指令前设置`ui_output_dir`变量，ui代码文件将在`${ui_output_dir}/${target}`目录下生成  
    例如：如果想要在根目录的ui文件夹下生成ui代码，则需要将`ui_output_dir`设置为`${CMAKE_CURRENT_SOURCE_DIR}`，
    并且ui的`target`名称需要设置为`ui`  
    然后在你的cmake文件中使用`addUI`指令添加ui target，其函数签名如下
    ```cmake
    addUI(<target> <type> <ui_file> [<frame> ...])
    ```
    * `<target>`: 目标名称，ui代码将会在`${ui_output_dir}/${target}`目录下生成
    * `<type>`: 目标类型，支持`dynamic`和`static`两种类型，对应静态版和动态版代码
    * `<ui_file>`: rmui文件路径，支持相对路径和绝对路径，相对路径以当前cmake文件路径起始
    * `<frame>`: 可选参数，要生成的frame的名称，留空为`default`
3. 链接
    生成的ui代码需要链接到你的工程中，链接方式和普通的cmake目标一样，使用`target_link_libraries`指令即可
    ```cmake
    target_link_libraries(<your_target> <ui_target>)
    ```
    在链接以后，`ui_output_dir`目录将被自动添加到`include`路径，使用`#include "<target>/ui.h"`即可访问到ui
4. 使用
    具体使用与ui生成器生成代码相同，详见[动态版使用指南](https://bbs.robomaster.com/article/580951)和
    [静态版使用指南](https://bbs.robomaster.com/article/558118)  
    唯一需要注意的是由于代码可能重复生成，建议只修改`ui_types.h`和`ui_interface.h`中被`// User Code`包裹的部分，
    这些内容在重新生成时不会被覆盖。如果修改`// User Code`注释可能会导致不可预料的结果

### 常见问题
* 我的平台会自动下载预购建生成器但其无法正常运行怎么办？  
  可以在`include(addUI.cmake)`前设置`ui_compile_generator`变量为`TRUE`，
  将优先识别`${CMAKE_CURRENT_BINARY_DIR}/rm_ui_generator(.exe)`文件是否可用，如果不可用则下载源码进行编译
* 能否在无网络条件下从源码构建生成器？  
  暂时不能在无网条件下自动构建生成器，需手动构建生成器后将可执行文件移动到
  `${CMAKE_CURRENT_BINARY_DIR}/rm_ui_generator(.exe)`位置下，程序会自动采用
* 有没有示例可供参考？  
  仓库中中`test`目录下有一个完整的示例工程，提供了静态版和动态版的使用示例（该实例假设将在windows x64平台上运行并使用C++编码，
  如需在单片机上运行请调整`ui_interface.h`中的`SEND_MESSAGE`宏，采用C++编码仅为了方便，UI相关API与C相同）