# Erle_copter 仿真安装步骤

> # 说明：
>
> - 操作系统：ubuntu16
> - ROS：kinetic
> - Gazebo：8.6

## 一、安装gazebo8

```python
$ sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
$ wget http://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
$ sudo apt-get update
$ sudo apt-get install gazebo8
$ sudo apt-get install libgazebo8-dev
```



## 二、安装ros kinetic （不安装desktop-full版本）

```python
$ sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
$ sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
$ sudo apt-get update
$ sudo apt-get install ros-kinetic-desktop
$ sudo rosdep init
$ rosdep update
$ echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
$ source ~/.bashrc
$ sudo apt-get install python-rosinstall python-rosinstall-generator python-wstool build-essential
```

## 三、安装必要ROS程序包

```python
$ sudo apt-get install ros-kinetic-gazebo8-msgs
$ sudo apt-get install ros-kinetic-gazebo8-ros-control
$ sudo apt-get install ros-kinetic-gazebo8-plugins
$ sudo apt-get install ros-kinetic-gazebo8-ros-pkgs
$ sudo apt-get install ros-kinetic-gazebo8-ros
$ sudo apt-get install ros-kinetic-image-view

$ sudo apt-get install ros-kinetic-mavlink
$ sudo apt-get install ros-kinetic-octomap-msgs
$ sudo apt-get install libgoogle-glog-dev protobuf-compiler ros-$ROS_DISTRO-octomap-msgs ros-$ROS_DISTRO-octomap-ros ros-$ROS_DISTRO-joy
$ sudo apt-get install libtool automake autoconf libexpat1-dev
$ sudo apt-get install ros-kinetic-mavros-msgs
$ sudo apt-get install ros-kinetic-gazebo-msgs
```

## 四、安装erle_copter仿真环境

> 安装基础包

```python
$ sudo apt-get update
$ sudo apt-get install gawk make git curl cmake -y
```

> 安装MAVProxy依赖

```python
$ sudo apt-get install g++ python-pip python-matplotlib python-serial python-wxgtk2.8 python-scipy python-opencv python-numpy python-pyparsing ccache realpath libopencv-dev -y
```

> 安装MAVProxy

```python
$ sudo pip install future
$ sudo apt-get install libxml2-dev libxslt1-dev -y
$ sudo pip2 install pymavlink catkin_pkg --upgrade
$ sudo pip install MAVProxy==1.5.2
```

> clone 文件

```python
$ git clone https://github.com/ldgcug/erlecopter_gazebo8.git
```

> 安装ArUco

```python
$ cp -r ~/erlecopter_gazebo8/aruco-1.3.0/ ~/Downloads/
$ cd ~/Downloads/aruco-1.3.0/build
$ cmake ..
$ make
$ sudo make install
说明：如果 cmake ..  或 make 等报错，则删除build文件，重新创建build文件并编译，具体操作如下：
$ cd ~/Downloads/aruco-1.3.0/
$ rm -rf build/
$ mkdir build && cd build
$ cmake ..
$ make
$ sudo make install
```

> 拷贝ardupilot到特定文件夹

```python
$ mkdir -p ~/simulation; cd ~/simulation
$ cp -r ~/erlecopter_gazebo8/ardupilot/ ~/simulation/
```

> 创建ros工作空间及初始化工作空间

```
$ mkdir -p ~/simulation/ros_catkin_ws/src
$ cd ~/simulation/ros_catkin_ws/src
$ catkin_init_workspace
$ cd ~/simulation/ros_catkin_ws
$ catkin_make
$ source devel/setup.bash
```

> 拷贝相关源码到工作空间内

```python
$ cp -r ~/erlecopter_gazebo8/ardupilot_sitl_gazebo_plugin/  ~/simulation/ros_catkin_ws/src/ 
$ cp -r ~/erlecopter_gazebo8/hector_gazebo/  ~/simulation/ros_catkin_ws/src/ 
$ cp -r ~/erlecopter_gazebo8/rotors_simulator/ ~/simulation/ros_catkin_ws/src/ 
$ cp -r ~/erlecopter_gazebo8/mav_comm/ ~/simulation/ros_catkin_ws/src/ 
$ cp -r ~/erlecopter_gazebo8/glog_catkin/ ~/simulation/ros_catkin_ws/src/ 
$ cp -r ~/erlecopter_gazebo8/catkin_simple/ ~/simulation/ros_catkin_ws/src/ 
$ cp -r ~/erlecopter_gazebo8/mavros/ ~/simulation/ros_catkin_ws/src/
$ cp -r ~/erlecopter_gazebo8/gazebo_ros_pkgs/ ~/simulation/ros_catkin_ws/src/

添加Python和C++样例
$ cp -r ~/erlecopter_gazebo8/gazebo_cpp_examples/ ~/simulation/ros_catkin_ws/src/
$ cp -r ~/erlecopter_gazebo8/gazebo_python_examples/ ~/simulation/ros_catkin_ws/src/
```

> 拷贝fix-unused-typedef-warning.patch文件到工作空间内
```python
$ cp -r ~/erlecopter_gazebo8/fix-unused-typedef-warning.patch ~/simulation/ros_catkin_ws/src/
```
> 安装drcsim7（ubuntu16不支持apt-get，需使用源码下载安装）

```python
$ sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu xenial main" > /etc/apt/sources.list.d/ros-latest.list'
$ wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
$ sudo apt-get update

# Install osrf-common's dependencies
$ sudo apt-get install -y cmake               \
                        debhelper           \
                        ros-kinetic-ros      \
                        ros-kinetic-ros-comm
                        
# Install sandia-hand's dependencies
$ sudo apt-get install -y ros-kinetic-xacro        \
                        ros-kinetic-ros          \
                        ros-kinetic-image-common \
                        ros-kinetic-ros-comm     \
                        ros-kinetic-common-msgs  \
                        libboost-dev            \
                        avr-libc                \
                        gcc-avr                 \
                        libqt4-dev
                        
 # Install gazebo-ros-pkgs
 $ sudo apt-get install -y libtinyxml-dev                 \
                        ros-kinetic-opencv3             \
                        ros-kinetic-angles              \
                        ros-kinetic-cv-bridge           \
                        ros-kinetic-driver-base         \
                        ros-kinetic-dynamic-reconfigure \
                        ros-kinetic-geometry-msgs       \
                        ros-kinetic-image-transport     \
                        ros-kinetic-message-generation  \
                        ros-kinetic-nav-msgs            \
                        ros-kinetic-nodelet             \
                        ros-kinetic-pcl-conversions     \
                        ros-kinetic-pcl-ros             \
                        ros-kinetic-polled-camera       \
                        ros-kinetic-rosconsole          \
                        ros-kinetic-rosgraph-msgs       \
                        ros-kinetic-sensor-msgs         \
                        ros-kinetic-trajectory-msgs     \
                        ros-kinetic-urdf                \
                        ros-kinetic-dynamic-reconfigure \
                        ros-kinetic-rosgraph-msgs       \
                        ros-kinetic-tf                  \
                        ros-kinetic-cmake-modules  
# Install drcsim's dependencies   
$ sudo apt-get install -y cmake debhelper                         \
                     ros-kinetic-std-msgs ros-kinetic-common-msgs   \
                     ros-kinetic-image-common ros-kinetic-geometry  \

                     ros-kinetic-robot-state-publisher            \
                     ros-kinetic-image-pipeline                   \
                     ros-kinetic-image-transport-plugins          \
                     ros-kinetic-compressed-depth-image-transport \
                     ros-kinetic-compressed-image-transport       \
                     ros-kinetic-theora-image-transport           \
                     ros-kinetic-ros-controllers                  \
                     ros-kinetic-moveit-msgs                      \
                     ros-kinetic-joint-limits-interface           \
                     ros-kinetic-transmission-interface           \
                     ros-kinetic-laser-assembler                               
$ sudo apt-get install ros-kinetic-pr2-controllers                  
```

> 拷贝drcsim相关包到工作空间

```python
$ cp -r ~/erlecopter_gazebo8/osrf-common/  ~/simulation/ros_catkin_ws/src/
$ cp -r ~/erlecopter_gazebo8/sandia-hand/ ~/simulation/ros_catkin_ws/src/
$ cp -r ~/erlecopter_gazebo8/drcsim/ ~/simulation/ros_catkin_ws/src/
```

> source一下

```python
$ source /opt/ros/kinetic/setup.bash
```

> 修改 has_binary_operator.hpp文件（为避免包BOOST_JOIN错误）

```python
$ sudo gedit /usr/include/boost/type_traits/detail/has_binary_operator.hpp
```

将如下代码复制，并粘贴至has_binary_operator.hpp 文件中

主要的修改是在两处位置添加了 `#ifndef Q_MOC_RUN` 和`#endif`

```python
//  (C) Copyright 2009-2011 Frederic Bron, Robert Stewart, Steven Watanabe & Roman Perepelitsa.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#include <boost/config.hpp>
#include <boost/type_traits/ice.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>

// should be the last #include
#include <boost/type_traits/detail/bool_trait_def.hpp>

// cannot include this header without getting warnings of the kind:
// gcc:
//    warning: value computed is not used
//    warning: comparison between signed and unsigned integer expressions
// msvc:
//    warning C4018: '<' : signed/unsigned mismatch
//    warning C4244: '+=' : conversion from 'double' to 'char', possible loss of data
//    warning C4547: '*' : operator before comma has no effect; expected operator with side-effect
//    warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
//    warning C4804: '<' : unsafe use of type 'bool' in operation
//    warning C4805: '==' : unsafe mix of type 'bool' and type 'char' in operation
// cannot find another implementation -> declared as system header to suppress these warnings.
#if defined(__GNUC__)
#   pragma GCC system_header
#elif defined(BOOST_MSVC)
#   pragma warning ( push )
#   pragma warning ( disable : 4018 4244 4547 4800 4804 4805 4913 )
#endif

namespace boost {
namespace detail {

// This namespace ensures that argument-dependent name lookup does not mess things up.
#ifndef Q_MOC_RUN
namespace BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {
#endif
// 1. a function to have an instance of type T without requiring T to be default
// constructible
template <typename T> T &make();


// 2. we provide our operator definition for types that do not have one already

// a type returned from operator BOOST_TT_TRAIT_OP when no such operator is
// found in the type's own namespace (our own operator is used) so that we have
// a means to know that our operator was used
struct no_operator { };

// this class allows implicit conversions and makes the following operator
// definition less-preferred than any other such operators that might be found
// via argument-dependent name lookup
struct any { template <class T> any(T const&); };

// when operator BOOST_TT_TRAIT_OP is not available, this one is used
no_operator operator BOOST_TT_TRAIT_OP (const any&, const any&);


// 3. checks if the operator returns void or not
// conditions: Lhs!=void and Rhs!=void

// we first redefine "operator," so that we have no compilation error if
// operator BOOST_TT_TRAIT_OP returns void and we can use the return type of
// (lhs BOOST_TT_TRAIT_OP rhs, returns_void_t()) to deduce if
// operator BOOST_TT_TRAIT_OP returns void or not:
// - operator BOOST_TT_TRAIT_OP returns void   -> (lhs BOOST_TT_TRAIT_OP rhs, returns_void_t()) returns returns_void_t
// - operator BOOST_TT_TRAIT_OP returns !=void -> (lhs BOOST_TT_TRAIT_OP rhs, returns_void_t()) returns int
struct returns_void_t { };
template <typename T> int operator,(const T&, returns_void_t);
template <typename T> int operator,(const volatile T&, returns_void_t);

// this intermediate trait has member value of type bool:
// - value==true -> operator BOOST_TT_TRAIT_OP returns void
// - value==false -> operator BOOST_TT_TRAIT_OP does not return void
template < typename Lhs, typename Rhs >
struct operator_returns_void {
   // overloads of function returns_void make the difference
   // yes_type and no_type have different size by construction
   static ::boost::type_traits::yes_type returns_void(returns_void_t);
   static ::boost::type_traits::no_type returns_void(int);
   BOOST_STATIC_CONSTANT(bool, value = (sizeof(::boost::type_traits::yes_type)==sizeof(returns_void((make<Lhs>() BOOST_TT_TRAIT_OP make<Rhs>(),returns_void_t())))));
};


// 4. checks if the return type is Ret or Ret==dont_care
// conditions: Lhs!=void and Rhs!=void

struct dont_care { };

template < typename Lhs, typename Rhs, typename Ret, bool Returns_void >
struct operator_returns_Ret;

template < typename Lhs, typename Rhs >
struct operator_returns_Ret < Lhs, Rhs, dont_care, true > {
   BOOST_STATIC_CONSTANT(bool, value = true);
};

template < typename Lhs, typename Rhs >
struct operator_returns_Ret < Lhs, Rhs, dont_care, false > {
   BOOST_STATIC_CONSTANT(bool, value = true);
};

template < typename Lhs, typename Rhs >
struct operator_returns_Ret < Lhs, Rhs, void, true > {
   BOOST_STATIC_CONSTANT(bool, value = true);
};

template < typename Lhs, typename Rhs >
struct operator_returns_Ret < Lhs, Rhs, void, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename Lhs, typename Rhs, typename Ret >
struct operator_returns_Ret < Lhs, Rhs, Ret, true > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

// otherwise checks if it is convertible to Ret using the sizeof trick
// based on overload resolution
// condition: Ret!=void and Ret!=dont_care and the operator does not return void
template < typename Lhs, typename Rhs, typename Ret >
struct operator_returns_Ret < Lhs, Rhs, Ret, false > {
   static ::boost::type_traits::yes_type is_convertible_to_Ret(Ret); // this version is preferred for types convertible to Ret
   static ::boost::type_traits::no_type is_convertible_to_Ret(...); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(is_convertible_to_Ret(make<Lhs>() BOOST_TT_TRAIT_OP make<Rhs>()))==sizeof(::boost::type_traits::yes_type)));
};


// 5. checks for operator existence
// condition: Lhs!=void and Rhs!=void

// checks if our definition of operator BOOST_TT_TRAIT_OP is used or an other
// existing one;
// this is done with redefinition of "operator," that returns no_operator or has_operator
struct has_operator { };
no_operator operator,(no_operator, has_operator);

template < typename Lhs, typename Rhs >
struct operator_exists {
   static ::boost::type_traits::yes_type s_check(has_operator); // this version is preferred when operator exists
   static ::boost::type_traits::no_type s_check(no_operator); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(s_check(((make<Lhs>() BOOST_TT_TRAIT_OP make<Rhs>()),make<has_operator>())))==sizeof(::boost::type_traits::yes_type)));
};


// 6. main trait: to avoid any compilation error, this class behaves
// differently when operator BOOST_TT_TRAIT_OP(Lhs, Rhs) is forbidden by the
// standard.
// Forbidden_if is a bool that is:
// - true when the operator BOOST_TT_TRAIT_OP(Lhs, Rhs) is forbidden by the standard
//   (would yield compilation error if used)
// - false otherwise
template < typename Lhs, typename Rhs, typename Ret, bool Forbidden_if >
struct trait_impl1;

template < typename Lhs, typename Rhs, typename Ret >
struct trait_impl1 < Lhs, Rhs, Ret, true > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename Lhs, typename Rhs, typename Ret >
struct trait_impl1 < Lhs, Rhs, Ret, false > {
   BOOST_STATIC_CONSTANT(bool,
      value = (
         ::boost::type_traits::ice_and<
            operator_exists < Lhs, Rhs >::value,
            operator_returns_Ret < Lhs, Rhs, Ret, operator_returns_void < Lhs, Rhs >::value >::value
         >::value
      )
   );
};

// some specializations needs to be declared for the special void case
template < typename Rhs, typename Ret >
struct trait_impl1 < void, Rhs, Ret, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename Lhs, typename Ret >
struct trait_impl1 < Lhs, void, Ret, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename Ret >
struct trait_impl1 < void, void, Ret, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

// defines some typedef for convenience
template < typename Lhs, typename Rhs, typename Ret >
struct trait_impl {
   typedef typename ::boost::remove_reference<Lhs>::type Lhs_noref;
   typedef typename ::boost::remove_reference<Rhs>::type Rhs_noref;
   typedef typename ::boost::remove_cv<Lhs_noref>::type Lhs_nocv;
   typedef typename ::boost::remove_cv<Rhs_noref>::type Rhs_nocv;
   typedef typename ::boost::remove_cv< typename ::boost::remove_reference< typename ::boost::remove_pointer<Lhs_noref>::type >::type >::type Lhs_noptr;
   typedef typename ::boost::remove_cv< typename ::boost::remove_reference< typename ::boost::remove_pointer<Rhs_noref>::type >::type >::type Rhs_noptr;
   BOOST_STATIC_CONSTANT(bool, value = (trait_impl1 < Lhs_noref, Rhs_noref, Ret, BOOST_TT_FORBIDDEN_IF >::value));
};

#ifndef Q_MOC_RUN
} // namespace impl
#endif
} // namespace detail

// this is the accessible definition of the trait to end user
BOOST_TT_AUX_BOOL_TRAIT_DEF3(BOOST_TT_TRAIT_NAME, Lhs, Rhs=Lhs, Ret=::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::dont_care, (::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::trait_impl < Lhs, Rhs, Ret >::value))

} // namespace boost

#if defined(BOOST_MSVC)
#   pragma warning ( pop )
#endif

#include <boost/type_traits/detail/bool_trait_undef.hpp>
```

> 编译工作空间

```python
$ cd ~/simulation/ros_catkin_ws
$ catkin_make --pkg mav_msgs mavros_msgs gazebo_msgs
$ source devel/setup.bash
$ catkin_make -j 4
```

> 下载gazebo模型

```
$ mkdir -p ~/.gazebo/models
$ git clone https://github.com/erlerobot/erle_gazebo_models
$ mv erle_gazebo_models/* ~/.gazebo/models
```

## 五、启动erle_copter

> 启动ArduCopter（一个终端）

```
$ source ~/simulation/ros_catkin_ws/devel/setup.bash
$ cd ~/simulation/ardupilot/ArduCopter
$ ../Tools/autotest/sim_vehicle.sh -j 4 -f Gazebo
```

> 在另一个终端启动launch

```
$ cd ~/simulation/ros_catkin_ws/
$ source ~/simulation/ros_catkin_ws/devel/setup.bash
$ roslaunch ardupilot_sitl_gazebo_plugin erlecopter_spawn.launch
```

## 官网安装网址

>  http://docs.erlerobotics.com/simulation/configuring_your_environment

