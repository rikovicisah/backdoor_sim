#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#endif

static const char *SECRET = "letmein";
static const char *URL = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
// 0 - writeout, 1 - open browser
static const int OPEN_BROWSER = 1;

// creating file
static int get_temp_dir(char *buf, size_t cap)
{
#ifdef _WIN32
    DWORD len = GetTempPathA((DWORD)cap, buf);
    if (len == 0 || len > cap)
        return 0;
#else
    const char *tmp = getenv("TMPDIR");
    if (!tmp)
        tmp = "/tmp";
    if (snprintf(buf, cap, "%s", tmp) >= (int)cap)
        return 0;
#endif
    return 1;
}

//
static void timestamp(char *buf, size_t cap)
{
    time_t t = time(NULL);
    struct tm *tmv =
#ifdef _WIN32
        localtime(&t);
#else
        localtime(&t);
#endif
    strftime(buf, cap, "%Y-%m-%d %H:%M:%S", tmv);
}

static void write_log(const char *msg)
{
    char dir[512];
    char path[1024];
    if (!get_temp_dir(dir, sizeof(dir)))
        return;
#ifdef _WIN32
    snprintf(path, sizeof(path), "%s\\backdoor_sim.log", dir);
#else
    snprintf(path, sizeof(path), "%s/backdoor_sim.log", dir);
#endif
    FILE *f = fopen(path, "a");
    if (!f)
        return;
    char ts[64];
    timestamp(ts, sizeof(ts));
    fprintf(f, "[%s] %s\n", ts, msg);
    fclose(f);
}

static void open_url(const char *url)
{
    // Harmless “akcija” – otvori URL u default browseru (ili preskoči ako OPEN_BROWSER=0)
    if (!OPEN_BROWSER)
    {
        printf("[AKCIJA] Preskočeno otvaranje browsera. Poruka umjesto toga.\n");
        return;
    }
#ifdef _WIN32
    // Najjednostavnije: system("start ...") – bez citata sa spec-znakovima
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "start \"\" \"%s\"", url);
    system(cmd);
#else
    // Linux/Unix: koristi xdg-open (ako postoji)
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "xdg-open \"%s\" >/dev/null 2>&1 &", url);
    system(cmd);
#endif
}

int main(int argc, char **argv)
{
    printf("=== Backdoor demonstration ===\n");
    printf("Educational project.\n");

    // 1) “Aktivacija” šifrom
    if (argc < 2)
    {
        printf("Upotreba: %s <secret>\n", argv[0]);
        printf("Primjer:  %s %s\n", argv[0], SECRET);
        write_log("Pokrenut bez argumenata (nije aktiviran).");
        return 0;
    }
    if (strcmp(argv[1], SECRET) != 0)
    {
        printf("[INFO] Pogrešna šifra. Nema akcije.\n");
        write_log("Pokusaj aktivacije sa pogrešnom šifrom.");
        return 0;
    }

    // 2) Vidljiva “aktivacija”
    printf("[AKTIVIRANO] Simulacija backdoora aktivirana (DEMO).\n");
    write_log("Aktivirano ispravnom šifrom.");

    // 3) “Akcija” (harmless)
    printf("[AKCIJA] Otvaram YouTube (harmless) ili ispisujem poruku.\n");
    write_log("Izvedena harmless akcija.");

    open_url(URL);

    // 4) Završni trag
    printf("[KRAJ] Demo gotov. Pogledaj log u temp folderu.\n");
    write_log("Zavrsetak demonstracije.");
    return 0;
}