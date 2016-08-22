#include <fstream>
#include "mdtransform.hpp"


int main()
{
    // 装载构造 Markdown 文件
    MarkdownTransform transformer("test.md");

    /*
    // 编写一个 `getTableOfContents()` 方法来获取 Markdown 文件 HTML 格式的目录
    std::string table = transformer.getTableOfContents();

    // 编写一个 `getContents()` 方法来获取 Markdown 转成 HTML 后的内容
    std::string contents = transformer.getContents();

    // 准备要写入的 HTML 文件头尾信息
    std::string head = "<!DOCTYPE html><html><head>\
        <meta charset=\"utf-8\">\
        <title>Markdown</title>\
        <link rel=\"stylesheet\" href=\"github-markdown.css\">\
        </head><body><article class=\"markdown-body\">";
    std::string end = "</article></body></html>";

    // 将结果写入到文件
    std::ofstream out;
    out.open("output/index.html");
    // 将转换后的内容构造到 HTML 的 <article></article> 标签内部
    out << head+table+contents+end;
    out.close();
    */

    char src[] = "```    public static void main()";

    std::pair<int, char*> testpair;
    //testpair = transformer.start(src);

    //testpair = transformer.judgeType(src);


    return 0;
}

