#include "vk.h"

vk::vk()
{
    //ctor
}

vk::~vk()
{
    //dtor
}

string vk::getVideoLink(string str1, string str2, string str3, string str4){
    //http://cs532122v4.vk.me/u195176000/videos/a5dc2ff9e7.360.mp4

    string videourl = "";
    if (str1[(str1.length()-1)] == '/')
        videourl = str1 + "u" + str2 + "/videos/" + str3 + "." + str4;
    else
        videourl = str1 + "/u" + str2 + "/videos/" + str3 + "." + str4;

    return videourl;
}

string vk::download(string url){
    HttpUtil util;

    if (util.download(url)){
        //util.writeToFile("C:\\allmyvideos.net_8cf8gd6ez70t.htm");
        HtmlParser parser;
        //cout << parser.CleanHTML(util.getData()) << endl;
        Tags tag;
        tag.htmlTag = GUMBO_TAG_SCRIPT;
        //El video final esta en un campo llamado file: "URL" dentro de <script>
        string script = parser.buscarTextoInTag(util.getData(), &tag);
        string video_host = Constant::replaceAll(parser.searchString(&script, "var video_host = ", ";", false), "'", "");
        string video_uid = Constant::replaceAll(parser.searchString(&script, "var video_uid = ", ";", false), "'", "");
        string video_vtag = Constant::replaceAll(parser.searchString(&script, "var video_vtag = ", ";", false), "'", "");
        string video_no_flv = Constant::replaceAll(parser.searchString(&script, "var video_no_flv = ", ";", false), "'", "");
        string video_max_hd = Constant::replaceAll(parser.searchString(&script, "var video_max_hd = ", ";", false), "'", "");
        string vkid = Constant::replaceAll(parser.searchString(&script, "\"vkid\":", ",", false), "'", "");


        string urlVideo = "";
        if (video_no_flv.compare("0") == 0 && video_uid.compare("0") != 0){
            urlVideo = ((video_host.find("http://") == string::npos) ? "http://" : "") + video_host + "/u"
                    + video_uid + "/video/" + video_vtag + "." + "flv";
        } else if( video_uid.compare("0") == 0 && vkid.compare("") != 0){
            urlVideo = ((video_host.find("http://") == string::npos) ? "http://" : "") + video_host + "/assets/videos/"
                    + video_vtag + vkid + ".vk.flv";
        } else {
            if (video_max_hd.compare("0") == 0)
                urlVideo = getVideoLink(video_host,video_uid,video_vtag,"240.mp4");
            else if (video_max_hd.compare("1") == 0)
                urlVideo = getVideoLink(video_host,video_uid,video_vtag,"360.mp4");
            else if (video_max_hd.compare("2") == 0)
                urlVideo = getVideoLink(video_host,video_uid,video_vtag,"480.mp4");
            else if (video_max_hd.compare("3") == 0)
                urlVideo = getVideoLink(video_host,video_uid,video_vtag,"720.mp4");
            else
                urlVideo = getVideoLink(video_host,video_uid,video_vtag,"360.mp4");
        }

        return urlVideo;
    }
    return "";
}
