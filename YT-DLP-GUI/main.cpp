#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <windows.h>

#include <string>
#include <limits.h>
#include <iostream>

#include <sstream>
#include <string>
#include <fstream>

#include <vector>

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

#include <tlhelp32.h>

// change language this program - RU or EN
#define RU

using json = nlohmann::json;

typedef struct Formats_
{
    int id;
    int quality;
    int filesize;
} Formats;

Formats Get[9];

DWORD CheckTime = GetTickCount();

void ReadJson()
{
    //This is a pause for less consumption of hard disk resources
    if (GetTickCount() - CheckTime < 500)
        return;
    CheckTime = GetTickCount();

    std::ifstream ifs("meta.json");

    //wait is downloaded
    if (!ifs.is_open())
        return;

    //if files is empty
    bool isEmpty = ifs.peek() == EOF;
    if (isEmpty) return;

    json j = json::parse(ifs);

    int i = 0;
    for (auto& element : j["formats"])
    {
        //we are get ID from JSON without " "
        //we are need in the removing his
        std::string GetId = to_string(j["formats"][i]["format_id"]);
        GetId.erase(0, 1);
        GetId.erase(GetId.length(), 1);
        int id = atoi(GetId.c_str());

        std::string GetQuality = to_string(j["formats"][i]["format_note"]);
        GetQuality.erase(0, 1);
        GetQuality.erase(GetQuality.length(), 2);
        int quality = atoi(GetQuality.c_str());


        int filesize = atoi(to_string(j["formats"][i]["filesize"]).c_str());

        switch (atoi(GetQuality.c_str()))
        {
        case 144:
        {
            if (Get[0].filesize < filesize)
            {
                Get[0].id = id;
                Get[0].quality = quality;
                Get[0].filesize = filesize;
            }
            break;
        }
        case 240:
        {
            if (Get[1].filesize < filesize)
            {
                Get[1].id = id;
                Get[1].quality = quality;
                Get[1].filesize = filesize;
            }
            break;
        }
        case 360:
        {
            if (Get[2].filesize < filesize)
            {
                Get[2].id = id;
                Get[2].quality = quality;
                Get[2].filesize = filesize;
            }
            break;
        }
        case 480:
        {
            if (Get[3].filesize < filesize)
            {
                Get[3].id = id;
                Get[3].quality = quality;
                Get[3].filesize = filesize;
            }
            break;
        }
        case 720: //HD
        {
            if (Get[4].filesize < filesize)
            {
                Get[4].id = id;
                Get[4].quality = quality;
                Get[4].filesize = filesize;
            }
            break;
        }
        case 1080: //FullHD
        {
            if (Get[5].filesize < filesize)
            {
                Get[5].id = id;
                Get[5].quality = quality;
                Get[5].filesize = filesize;
            }
            break;
        }
        case 1440: //2k
        {
            if (Get[6].filesize < filesize)
            {
                Get[6].id = id;
                Get[6].quality = quality;
                Get[6].filesize = filesize;
            }
            break;
        }
        case 2160: //4k
        {
            if (Get[7].filesize < filesize)
            {
                Get[7].id = id;
                Get[7].quality = quality;
                Get[7].filesize = filesize;
            }
            break;
        }
        case 4320: //8k
        {
            if (Get[8].filesize < filesize)
            {
                Get[8].id = id;
                Get[8].quality = quality;
                Get[8].filesize = filesize;
            }
            break;
        }
        }
        i++;
    }
    ifs.close();
    return;
}

//We are get tmp.log from yt-dlp in encoding CP1251 (windows default), we are need change cp1251 to utf-8
//after, we are rendering text (ImGui supports UTF-8)
void cp1251_to_utf8(char* out, const char* in, unsigned int len)
{
    static const int table[128] =
    {
        // 80
        0x82D0,     0x83D0,     0x9A80E2,   0x93D1,     0x9E80E2,   0xA680E2,   0xA080E2,   0xA180E2,
        0xAC82E2,   0xB080E2,   0x89D0,     0xB980E2,   0x8AD0,     0x8CD0,     0x8BD0,     0x8FD0,
        // 90
        0x92D1,     0x9880E2,   0x9980E2,   0x9C80E2,   0x9D80E2,   0xA280E2,   0x9380E2,   0x9480E2,
        0,          0xA284E2,   0x99D1,     0xBA80E2,   0x9AD1,     0x9CD1,     0x9BD1,     0x9FD1,
        // A0
        0xA0C2,     0x8ED0,     0x9ED1,     0x88D0,     0xA4C2,     0x90D2,     0xA6C2,     0xA7C2,
        0x81D0,     0xA9C2,     0x84D0,     0xABC2,     0xACC2,     0xADC2,     0xAEC2,     0x87D0,
        // B0
        0xB0C2,     0xB1C2,     0x86D0,     0x96D1,     0x91D2,     0xB5C2,     0xB6C2,     0xB7C2,
        0x91D1,     0x9684E2,   0x94D1,     0xBBC2,     0x98D1,     0x85D0,     0x95D1,     0x97D1,
        // C0
        0x90D0,     0x91D0,     0x92D0,     0x93D0,     0x94D0,     0x95D0,     0x96D0,     0x97D0,
        0x98D0,     0x99D0,     0x9AD0,     0x9BD0,     0x9CD0,     0x9DD0,     0x9ED0,     0x9FD0,
        // D0
        0xA0D0,     0xA1D0,     0xA2D0,     0xA3D0,     0xA4D0,     0xA5D0,     0xA6D0,     0xA7D0,
        0xA8D0,     0xA9D0,     0xAAD0,     0xABD0,     0xACD0,     0xADD0,     0xAED0,     0xAFD0,
        // E0
        0xB0D0,     0xB1D0,     0xB2D0,     0xB3D0,     0xB4D0,     0xB5D0,     0xB6D0,     0xB7D0,
        0xB8D0,     0xB9D0,     0xBAD0,     0xBBD0,     0xBCD0,     0xBDD0,     0xBED0,     0xBFD0,
        // F0
        0x80D1,     0x81D1,     0x82D1,     0x83D1,     0x84D1,     0x85D1,     0x86D1,     0x87D1,
        0x88D1,     0x89D1,     0x8AD1,     0x8BD1,     0x8CD1,     0x8DD1,     0x8ED1,     0x8FD1
    };

    int count = 0;

    while (*in)
    {
        if (len && (count >= len)) break;

        if (*in & 0x80)
        {
            register int v = table[(int)(0x7f & *in++)];
            if (!v)
                continue;
            *out++ = (char)v;
            *out++ = (char)(v >> 8);
            if (v >>= 16)
                *out++ = (char)v;
        }
        else // ASCII
            *out++ = *in++;

        count++;
    }

    *out = 0;
}

//Its function reading tmp.log correcting some CR/LF errors and outputs a certain number of lines (now it's 15)
// CR - \r , LF - \n
std::string ReadFile(std::string file)
{
    std::string line;
    std::string linetmp;
    int lines = 0;
    int result_line = 0;
    std::string tmp;
    std::string res;

    int limitbound = 15;

    std::string bigdata;

    std::ifstream getlinesfile(file);
    if (getlinesfile.is_open())
    {
        while (std::getline(getlinesfile, linetmp))
        {
            if (linetmp[0] == '\r')
            {
                linetmp.erase(0, 1);
            }

            bigdata += linetmp + '\n';
            lines++;

        }
    }

    for (int i = 0; i <= bigdata.length(); i++)
    {
        if (bigdata[i] == '\r') //CR
        {
            bigdata[i] = '\n';
        }
    }

    const std::string s = bigdata;
    const char delim = '\n';

    std::vector<std::string> elems;

    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        result_line++;

        char test[1024];
        cp1251_to_utf8(test, item.c_str(), strlen(item.c_str()));

        item = test;

        if (ImGui::CalcTextSize(item.c_str()).x >= 580.0f)
        {
            limitbound--;
        }

        elems.push_back(item);
    }

    int count = 0;
    for (auto& it : elems)
    {
        for (int j = 0; j < limitbound; j++)
        {
            if (count == result_line - j)
                tmp += it + '\n';
        }
        count++;
    }

    return tmp;
}

// Its from ImGui
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    //disable resize window
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(600, 390, "YT-DLP-GUI v2 (by Vadim Boev)", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    //connect Font with russian characters
    static const ImWchar ranges[] =
    {
        0x0020, 0x0080,
        0x00A0, 0x00C0,
        0x0400, 0x0460,
        0x0490, 0x04A0,
        0x2010, 0x2040,
        0x20A0, 0x20B0,
        0x2110, 0x2130,
        0
    };
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 17.0f, NULL, ranges);

    //remove last log file
    remove("tmp.log");

    //background color
    ImVec4 clear_color = ImVec4(0.923f, 0.923f, 0.923f, 1.00f);

    //static string for InputText (there widget where input link)
    static char str0[512] = u8"";

    //changeble button, when link = Youtube
    //default = false
    bool smallbutton = false;

    //initialize
    for (int i = 0; i < 9; i++)
    {
        Get[i].id = 0;
        Get[i].filesize = 0;
        Get[i].quality = 0;
    }

    //save link for checking
    std::string now_link = "";

    //list quality
    const char* items[9] = { 0 };
    items[0] = "144p\0\0";
    items[1] = "144p\000240p\0\0";
    items[2] = "144p\000240p\000360p\0\0";
    items[3] = "144p\000240p\000360p\000480p\0\0";
    items[4] = "144p\000240p\000360p\000480p\000720p\0\0";
    items[5] = "144p\000240p\000360p\000480p\000720p\0001080p\0\0";
    items[6] = "144p\000240p\000360p\000480p\000720p\0001080p\0001440p\0\0";
    items[7] = "144p\000240p\000360p\000480p\000720p\0001080p\0001440p\0002160p\0\0";
    items[8] = "144p\000240p\000360p\000480p\000720p\0001080p\0001440p\0002160p\0004320p\0\0";

    //selected in combo (list quality)
    int item_current = 0;

    //selected Formats
    int selected_formats = 0;

    //show button for stop process downloading
    bool download_button = false;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;
            static float wrap_width = 200.0f;

            //its for render Demo Window ImGui
            //ImGui::ShowDemoWindow((bool*)true);

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(600, 400));

            ImGui::Begin("window", (bool*)true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing);

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
#ifdef EN
            ImGui::Text(u8"Insert a Youtube or TikTok link");
#else
            ImGui::Text(u8"Вставьте ссылку Youtube или TikTok");
#endif
            ImGui::PopStyleColor();

            //Set width for InputText
            ImGui::PushItemWidth(580);
            ImGui::InputText("link", str0, IM_ARRAYSIZE(str0));
            ImGui::PopItemWidth();

            //Set colors for button 'Download'
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(80, 200, 120, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(80, 240, 120, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 240, 120, 255));

            //small sh*t code
            std::string detectYT = str0;
            size_t pos = detectYT.find("youtu");
            if (pos != std::string::npos)
            {
                smallbutton = true;
            }
            else
            {
                smallbutton = false;
            }

            std::string download;

            ReadJson();

#ifdef EN
            download = "Download";
#else
            download = u8"Скачать";
#endif

            bool tmpButton = false;
            if (smallbutton)
            {
                if (now_link != detectYT)
                {
                    remove("meta.json");

                    //download json meta data
                    char tmp_mem[600] = "";
                    sprintf(tmp_mem, "/c yt-dlp_x86.exe -j \"%s\" > meta.json", str0);
                    ShellExecuteA(0, "open", "cmd.exe", tmp_mem, 0, SW_HIDE);

                    for (int j = 0; j < 9; j++)
                    {
                        Get[j] = {};
                    }

                    now_link = detectYT;
                }

                const char* getline = u8"Загрузка..";
                for (int k = 0; k < 9; k++)
                {
                    if (Get[k].id != 0)
                    {
                        getline = items[k];
                        selected_formats = k;
                    }
                }

                //set style and colors for combo
                ImGui::PushItemWidth(260.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 6.5f));
                ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(133, 178, 255, 255));
                ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(205, 234, 255, 255));
                ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(158, 228, 112, 255));
                ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(80, 200, 0, 255));
                ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(106, 200, 44, 255));
                ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(76, 190, 0, 255));

                ImGui::Combo("##combo", &item_current, getline);

                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();

                ImGui::SameLine();

                ImGui::SetCursorPosX(280.0f);
                tmpButton = ImGui::Button(download.c_str(), ImVec2(308, 30));
            }
            else
            {
                //else if TikTok link
                tmpButton = ImGui::Button(download.c_str(), ImVec2(580, 30));
            }

            if (tmpButton)
            {
                if (strlen(str0) == 0)
                {
#ifdef EN
                    MessageBox(0, "You didn't insert the link!", "Error!", MB_OK | MB_ICONERROR);
#else
                    MessageBox(0, "Вы не вставили ссылку!", "Ошибка!", MB_OK | MB_ICONERROR);
#endif
                }
                else
                {
                    bool success = false;
                    bool youtube = false;

                    //finding Youtube or TikTok links
                    //**Warning! Its sh*t code
                    std::string s = str0;
                    size_t pos = s.find("youtu");
                    if (pos != std::string::npos)
                    {
                        //todo
                        success = true;
                        youtube = true;
                    }
                    else
                    {
                        pos = s.find("tiktok.com");
                        if (pos != std::string::npos)
                        {
                            //todo
                            success = true;
                        }
                        else
                        {
#ifdef EN
                            MessageBox(0, "Unknown link!", "Error!", MB_OK | MB_ICONERROR);
#else
                            MessageBox(0, "Неизвестная ссылка!", "Ошибка!", MB_OK | MB_ICONERROR);
#endif
                        }
                    }
                    //end sh*t code

                    if (success)
                    {
                        //download video and save logs
                        char tmp_mem[600] = "";
                        if (youtube)
                            //sprintf(tmp_mem, "/c yt-dlp_x86.exe -f bestvideo[ext!=webm]+bestaudio[ext!=webm]/best[ext!=webm] --merge-output-format mp4 \"%s\" > tmp.log", str0);
                            sprintf(tmp_mem, "/c yt-dlp_x86.exe -f %d+bestaudio --merge-output-format mp4 \"%s\" > tmp.log", Get[item_current].id, str0);
                        else
                            sprintf(tmp_mem, "/c yt-dlp_x86.exe \"%s\" > tmp.log", str0);
                        ShellExecuteA(0, "open", "cmd.exe", tmp_mem, 0, SW_HIDE); //SW_SHOWMINIMIZED for button STOP
                    }
                }
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

#ifdef EN
            ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 15.0f, ImVec2(10, 100), IM_COL32_BLACK, u8"Download process:", NULL, 580.0f, NULL);
#else
            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 15.0f, ImVec2(10, 100), IM_COL32_BLACK, u8"Процесс скачивания:", NULL, 580.0f, NULL);
#endif

            ImGui::GetBackgroundDrawList()->AddRect(ImVec2(8, 118), ImVec2(588, 360), IM_COL32_BLACK, 0.0f);

            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 17.0f, ImVec2(10, 120), IM_COL32_BLACK, ReadFile("tmp.log").c_str(), NULL, 580.0f, NULL);

            //because VK is using mostly in Russia (and its borders abroad)
#ifdef RU
            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 15.0f, ImVec2(10, 370), IM_COL32_BLACK, u8"Автор программы в ВК:", NULL, 580.0f, NULL);
            ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 15.0f, ImVec2(160, 370), 0xFFF98100, u8"vk.com/kronka_vk", NULL, 580.0f, NULL);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(160, 386), ImVec2(268, 386), 0xFFF98100, 1.0f);

            //its for link to VK
            ImGui::SetCursorPos(ImVec2(160, 370));

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));

            if (ImGui::Button(" ", ImVec2(110, 17)))
            {
                ShellExecute(NULL, "open", "https://vk.com/kronka_vk", NULL, NULL, SW_SHOWNORMAL);
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(7);
            //end
#endif

            /*HANDLE hSnap;
            hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hSnap == NULL)
            {
                return 0;
            }
            PROCESSENTRY32 proc;

            if (Process32First(hSnap, &proc))
            {
                do {
                    if (strcmp(proc.szExeFile, "yt-dlp_x86.exe") == 0)
                    {
                        download_button = true;
                    }
                    else download_button = false;
                } while (Process32Next(hSnap, &proc));
            }

            if (download_button)
            {
                ImGui::SetCursorPos(ImVec2(400, 365));

                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(180, 0, 0, 255));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(200, 0, 0, 255));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(255, 0, 0, 255));

                if (ImGui::Button(u8"Остановить скачивание"))
                {
                    ShellExecuteA(0, "open", "cmd.exe", "/c taskkill.exe /f /im yt-dlp_x86.exe", 0, SW_HIDE);
                }

                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }*/

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
        // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
        //GLint last_program;
        //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        //glUseProgram(0);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        //glUseProgram(last_program);

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    remove("tmp.log");

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
