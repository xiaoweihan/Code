/*定义C++的名空间*/

namespace cpp ThriftModule


/*定义一个服务*/
service QueryService
{
   string QueryNameByID(1:i32 nID)

}

//定义另外一个服务
service ZipService
{
   void Zip()
}

