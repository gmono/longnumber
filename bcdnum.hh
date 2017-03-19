#include <string>
#include <list>
using namespace std;
class bcdcode
{
    #define callen(bytelen) bytelen/sizeof(long long )+(bytelen%sizeof(long long)!=0? 1:0)
    #define byteend ((char *)buf)+len-1
    #define bytestart (char *)buf;
    public:
    bcdcode()
    {
    }
    //此处假设string为大端存储因此取反向迭代器
    bcdcode(string str)
    {
        Init(str.rbegin(),str.rend(),str.length());
    }
//这是各种整数的转换缓冲大小计算函数 针对性特化
bcdcode(long long num){InitfromNumber(num);}
bcdcode(int num){InitfromNumber(num);}


    operator string()
    {
        string ret;
        char *eptr=byteend;
        char *sptr=bytestart;
        for(;eptr>=sptr;--eptr)
        {
            ret.push_back((*eptr)+'0');
        }
        return ret;
    }
    public:
    //计算函数系列
    bcdcode operator +(const bcdcode &num)
    {
        //直接加法 不进位
        int nlen=max(len,num.len);
        char *nbuf=len>num.len? (char *)buf:(char *)num.buf;
        bcdcode ret;//直接返回 则为Move语义
        ret.len=nlen;
        ret.buf=new long long[callen(nlen+1)]; //分配空间时 预先准备最高位仅为的空间
        //缓冲区视图
        char *rbuf=(char *)ret.buf;
        char *tbuf=(char *)buf;
        char *abuf=(char *)num.buf;
        //计算
        int mlen=min(len,num.len);
        for(int i=0;i<mlen;++i)
        {
            rbuf[i]=tbuf[i]+abuf[i];
        }
        //前面的复制
        for(int i=mlen;i<nlen;++i)
        {
            rbuf[i]=nbuf[i];
        }
        ret.add_update();//调整
        return ret;
    }


    private:
            //通用的序列到数字的转换初始化函数
        //默认为小端存储
    template<typename Itr>
    void Init(Itr begin,Itr end,int bytelen)
    {
        //此处应该保证buf为nullptr 且 len为0
        #ifdef __DEBUG
        if(buf) throw "错误，存储池不为空";
        if(buf==nullptr&&len!=0) throw "错误，内部参数不匹配";
        #endif
        //这里假设从begin到end为从低位到高位
        len=bytelen;
        buf=new long long[callen(bytelen)];
        char *cptr=bytestart;
        for(auto i=begin;i!=end;++i)
        {
            char c=*i;
            #ifdef __DEBUG
            if(c<='0'||c>='9') throw "错误 字符超界";
            #endif
            char bcd=c-'0';
            *cptr++=bcd;
        }
    }
    private:
    //通用数字转换函数
    #define ll_len(type) 22/sizeof(type)+(22%sizeof(type )!=0? 1:0)
    template<typename T>
    void InitfromNumber(T number)
    {
        //缓冲区 小端存储

        char *tbuf=new char[ll_len(T)];
        int nowlen=0;
        for(T i=number;i>0;i/=10)
        {
            tbuf[nowlen++]=(char )(i%10);
        }
        buf=(long long *)tbuf;
        len=nowlen;
    }
        
    //测试第四位的
    static constexpr char testbyte=0b00010000;
    static constexpr char setbyte=0b11101111;
    void add_update()
    {
        //加法减法的十进制调整
        char *cptr=bytestart;
        for(int i=0;i<len;++i)
        {
            char &c=cptr[i];
            if(c&testbyte)
            {
                if(i==len-1)
                {
                    //此处为最高位
                    cptr[i+1]=0;
                    len++;
                }
                c&=setbyte;
                cptr[i+1]++;
            }
        }
    }
    long long *buf=nullptr;//小端存储 低地址为低位 高地址高位
    int len=0;
};
