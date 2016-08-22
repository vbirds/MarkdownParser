
#ifndef MD2HTML
#define MD2HTML

#include <string.h>
#include <string>
#include <vector>
#include <utility>

#define maxLength 10000

// 词法关键字枚举
enum{
    nul             = 0,
    paragraph       = 1,
    href            = 2,
    ul              = 3,
    ol              = 4,
    li              = 5,
    em              = 6,
    strong          = 7,
    hr              = 8,
    br              = 9,
    image           = 10,
    quote           = 11,
    h1              = 12,
    h2              = 13,
    h3              = 14,
    h4              = 15,
    h5              = 16,
    h6              = 17,
    blockcode       = 18,
    code            = 19
};

// HTML 前置标签
const std::string frontTag[] = {
    "","<p>","","<ul>","<ol>","<li>","<em>","<strong>",
    "<hr color=#CCCCCC size=1 />","<br />",
    "","<blockquote>",
    "<h1 ","<h2 ","<h3 ","<h4 ","<h5 ","<h6 ", // 右边的尖括号预留给添加其他的标签属性, 如 id
    "<pre><code>","<code>"
};

// HTML 后置标签
const std::string backTag[] = {
    "","</p>","","</ul>","</ol>","</li>","</em>","</strong>",
    "","","","</blockquote>",
    "</h1>","</h2>","</h3>","</h4>","</h5>","</h6>",
    "</code></pre>","</code>"
};

//保存目录的结构
typedef struct Cnode
{
    std::vector<Cnode *> ch;
    std::string heading;
    std::string tag;

    Cnode (const std::string & hd) : heading(hd) {}

} Cnode;

typedef struct node
{
    int type;   //节点代表类型
    std::vector<node *> ch;

    // 用来存放三个重要的属性, elem[0] 保存了要显示的内容
    //elem[1] 保存连接 elem[2] 保存了 title
    std::string elem[3];

    node (int _type) : type(_type) {}

} node;

class MarkdownTransform{
private:
    node *root, *now;
    Cnode *Croot;
    std::string content, TOC;
    int cntTag = 0;
    char s[maxLength];

public:
    /*!
     * \brief start  开始解析一行中开始的空格和 Tab
     * \param src    源串
     * \return       由空格数和有内容处的 char* 指针组成的 std::pair
     */
    inline std::pair<int, char*> start(char *src)
    {
        if (src == nullptr ||  0 == (int)strlen(src))
        {
            return std::make_pair(0, nullptr);

        }

        //统计空格键和Tab键的个数
        int cntspace = 0, cnttab = 0;
        for (int i = 0; src[i] != '\0'; ++i)
        {
            if ( ' ' == src[i])
            {
                ++cntspace;
                continue;
            }
            else if ('\t' == src[i])
            {
                ++cnttab;
                continue;
            }

            // 如果内容前有空格和 Tab，那么将其统一按 Tab 的个数处理,
            // 其中, 一个 Tab = 四个空格
            return std::make_pair(cnttab + cntspace / 4, src + i);
        }

        return std::make_pair(0, nullptr);
    }

    /*!
     * \brief judgeType     判断当前行的类型
     * \param str           源串
     * \return              当前行的类型和除去行标志性关键字的正是内容的 char* 指针组成的 std::pair
     */
    inline std::pair<int, char*> judgeType(char * src)
    {
        char * ptr = src;

        //跳过 '#'
        while ('#' == *ptr)
        {
            ++ptr;
        }

        // 如果出现空格, 则说明是 `<h>` 标签
        if (ptr - src > 0 && ' ' == *ptr)
        {
            return std::make_pair(ptr - src + h1 - 1, ptr + 1);
        }

        ptr = src;
        // 如果出现 ``` 则说明是代码块
        if (0 == strncmp(ptr, "```", 3))
        {
            return std::make_pair(blockcode, ptr + 3);
        }

        // 如果出现 * + -, 并且他们的下一个字符为空格，则说明是列表
        if (0 == strncmp(ptr, "- ", 2) || \
            0 == strncmp(ptr, "* ", 2) || \
            0 == strncmp(ptr, "+ ", 2) )
        {
            return std::make_pair(ul, ptr + 1);
        }

        // 如果出现的是数字, 且下一个字符是 . 则说明是是有序列表
        char *ptr1 = ptr;
        while (*ptr1 && (isdigit(*ptr1)))
        {
            ++ptr1;
        }
        if (ptr1 != ptr && *ptr1 == '.' && ptr1[1] == ' ')
        {
             return std::make_pair(ol, ptr1 + 1);
        }

        // 如果出现 > 且下一个字符为空格，则说明是引用
        if ('>' == *ptr && (' ' == ptr[1]))
        {
            return std::make_pair(quote, ptr + 1);
        }

        // 否则，就是普通段落
        return std::make_pair(paragraph, ptr);
    }

    // 判断是否为标题
    inline bool isHeading(node *v)
    {
        return (v->type >= h1 && v->type <= h6);
    }
    // 判断是否为图片
    inline bool isImage(node *v)
    {
        return (v->type == image);
    }
    // 判断是否为超链接
    inline bool isHref(node *v)
    {
        return (v->type == href);
    }

public:
    // 构造函数
    MarkdownTransform(const std::string &filename){}

    // 获得 Markdown 目录
    std::string getTableOfContents() { return TOC; }
    // 获得 Markdown 内容
    std::string getContents() { return content; }

    // 析构函数
    ~MarkdownTransform(){}
};
#endif
