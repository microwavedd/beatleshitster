#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <atomic>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>
#endif

using namespace std;

atomic<bool> audio_playing(false);
#ifdef _WIN32
HANDLE hProcess = NULL;
#else
pid_t audio_pid = -1;
#endif

class Cancion {
public:
    string titulo;
    string album;
    int pista;
    string ruta_audio;

    Cancion(string t, string a, int p, string r)
    : titulo(t), album(a), pista(p), ruta_audio(r) {}
};

class Jugador {
public:
    string nombre;
    int fichas;
    vector<Cancion> acertadas;

    Jugador(string n) : nombre(n), fichas(0) {}
};

void reproducir_audio(const string& ruta) {
    #ifdef _WIN32
    string comando = "start /B vlc --intf dummy --play-and-exit \"" + ruta + "\"";
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    CreateProcess(NULL, (LPSTR)comando.c_str(), NULL, NULL, FALSE,
                  0, NULL, NULL, &si, &pi);
    hProcess = pi.hProcess;
    #else
    audio_pid = fork();
    if(audio_pid == 0) {
        execlp("cvlc", "cvlc", "--play-and-exit", ruta.c_str(), NULL);
        exit(0);
    }
    #endif
    audio_playing = true;
}

void detener_audio() {
    #ifdef _WIN32
    if(hProcess) {
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
        hProcess = NULL;
    }
    #else
    if(audio_pid != -1) {
        kill(audio_pid, SIGTERM);
        audio_pid = -1;
    }
    #endif
    audio_playing = false;
}

void mostrar_controles() {
    cout << "\nComandos especiales:\n";
    cout << "  stop - Detener música actual\n";
    cout << "  exit - Salir del juego\n";
    cout << "  pista [número] - Adivinar posición en álbum\n";
}

string to_lower(const string& str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

void limpiar_pantalla() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

int main() {
    srand(time(0));
    
    vector<Cancion> canciones = {
        // Please Please Me (1963)
        {"I Saw Her Standing There", "Please Please Me", 1, "ppm_1.mp3"},
        {"Misery", "Please Please Me", 2, "ppm_2.mp3"},
        {"Anna (Go to Him)", "Please Please Me", 3, "ppm_3.mp3"},
        {"Chains", "Please Please Me", 4, "ppm_4.mp3"},
        {"Boys", "Please Please Me", 5, "ppm_5.mp3"},
        {"Ask Me Why", "Please Please Me", 6, "ppm_6.mp3"},
        {"Please Please Me", "Please Please Me", 7, "ppm_7.mp3"},
        {"Love Me Do", "Please Please Me", 8, "ppm_8.mp3"},
        {"P.S. I Love You", "Please Please Me", 9, "ppm_9.mp3"},
        {"Baby It's You", "Please Please Me", 10, "ppm_10.mp3"},
        {"Do You Want to Know a Secret", "Please Please Me", 11, "ppm_11.mp3"},
        {"A Taste of Honey", "Please Please Me", 12, "ppm_12.mp3"},
        {"There's a Place", "Please Please Me", 13, "ppm_13.mp3"},
        {"Twist and Shout", "Please Please Me", 14, "ppm_14.mp3"},
    
        // With the Beatles (1963)
        {"It Won't Be Long", "With the Beatles", 1, "wtb_1.mp3"},
        {"All I've Got to Do", "With the Beatles", 2, "wtb_2.mp3"},
        {"All My Loving", "With the Beatles", 3, "wtb_3.mp3"},
        {"Don't Bother Me", "With the Beatles", 4, "wtb_4.mp3"},
        {"Little Child", "With the Beatles", 5, "wtb_5.mp3"},
        {"Till There Was You", "With the Beatles", 6, "wtb_6.mp3"},
        {"Please Mister Postman", "With the Beatles", 7, "wtb_7.mp3"},
        {"Roll Over Beethoven", "With the Beatles", 8, "wtb_8.mp3"},
        {"Hold Me Tight", "With the Beatles", 9, "wtb_9.mp3"},
        {"You Really Got a Hold on Me", "With the Beatles", 10, "wtb_10.mp3"},
        {"I Wanna Be Your Man", "With the Beatles", 11, "wtb_11.mp3"},
        {"Devil in Her Heart", "With the Beatles", 12, "wtb_12.mp3"},
        {"Not a Second Time", "With the Beatles", 13, "wtb_13.mp3"},
        {"Money (That's What I Want)", "With the Beatles", 14, "wtb_14.mp3"},
    
        // A Hard Day's Night (1964)
        {"A Hard Day's Night", "A Hard Day's Night", 1, "hdn_1.mp3"},
        {"I Should Have Known Better", "A Hard Day's Night", 2, "hdn_2.mp3"},
        {"If I Fell", "A Hard Day's Night", 3, "hdn_3.mp3"},
        {"I'm Happy Just to Dance with You", "A Hard Day's Night", 4, "hdn_4.mp3"},
        {"And I Love Her", "A Hard Day's Night", 5, "hdn_5.mp3"},
        {"Tell Me Why", "A Hard Day's Night", 6, "hdn_6.mp3"},
        {"Can't Buy Me Love", "A Hard Day's Night", 7, "hdn_7.mp3"},
        {"Any Time at All", "A Hard Day's Night", 8, "hdn_8.mp3"},
        {"I'll Cry Instead", "A Hard Day's Night", 9, "hdn_9.mp3"},
        {"Things We Said Today", "A Hard Day's Night", 10, "hdn_10.mp3"},
        {"When I Get Home", "A Hard Day's Night", 11, "hdn_11.mp3"},
        {"You Can't Do That", "A Hard Day's Night", 12, "hdn_12.mp3"},
        {"I'll Be Back", "A Hard Day's Night", 13, "hdn_13.mp3"},
    
        // Beatles for Sale (1964)
        {"No Reply", "Beatles for Sale", 1, "bfs_1.mp3"},
        {"I'm a Loser", "Beatles for Sale", 2, "bfs_2.mp3"},
        {"Baby's in Black", "Beatles for Sale", 3, "bfs_3.mp3"},
        {"Rock and Roll Music", "Beatles for Sale", 4, "bfs_4.mp3"},
        {"I'll Follow the Sun", "Beatles for Sale", 5, "bfs_5.mp3"},
        {"Mr. Moonlight", "Beatles for Sale", 6, "bfs_6.mp3"},
        {"Kansas City/Hey-Hey-Hey-Hey!", "Beatles for Sale", 7, "bfs_7.mp3"},
        {"Eight Days a Week", "Beatles for Sale", 8, "bfs_8.mp3"},
        {"Words of Love", "Beatles for Sale", 9, "bfs_9.mp3"},
        {"Honey Don't", "Beatles for Sale", 10, "bfs_10.mp3"},
        {"Every Little Thing", "Beatles for Sale", 11, "bfs_11.mp3"},
        {"I Don't Want to Spoil the Party", "Beatles for Sale", 12, "bfs_12.mp3"},
        {"What You're Doing", "Beatles for Sale", 13, "bfs_13.mp3"},
        {"Everybody's Trying to Be My Baby", "Beatles for Sale", 14, "bfs_14.mp3"},
    
        // Help! (1965)
        {"Help!", "Help!", 1, "help_1.mp3"},
        {"The Night Before", "Help!", 2, "help_2.mp3"},
        {"You've Got to Hide Your Love Away", "Help!", 3, "help_3.mp3"},
        {"I Need You", "Help!", 4, "help_4.mp3"},
        {"Another Girl", "Help!", 5, "help_5.mp3"},
        {"You're Going to Lose That Girl", "Help!", 6, "help_6.mp3"},
        {"Ticket to Ride", "Help!", 7, "help_7.mp3"},
        {"Act Naturally", "Help!", 8, "help_8.mp3"},
        {"It's Only Love", "Help!", 9, "help_9.mp3"},
        {"You Like Me Too Much", "Help!", 10, "help_10.mp3"},
        {"Tell Me What You See", "Help!", 11, "help_11.mp3"},
        {"I've Just Seen a Face", "Help!", 12, "help_12.mp3"},
        {"Yesterday", "Help!", 13, "help_13.mp3"},
        {"Dizzy Miss Lizzy", "Help!", 14, "help_14.mp3"},
    
        // Rubber Soul (1965)
        {"Drive My Car", "Rubber Soul", 1, "rs_1.mp3"},
        {"Norwegian Wood (This Bird Has Flown)", "Rubber Soul", 2, "rs_2.mp3"},
        {"You Won't See Me", "Rubber Soul", 3, "rs_3.mp3"},
        {"Nowhere Man", "Rubber Soul", 4, "rs_4.mp3"},
        {"Think for Yourself", "Rubber Soul", 5, "rs_5.mp3"},
        {"The Word", "Rubber Soul", 6, "rs_6.mp3"},
        {"Michelle", "Rubber Soul", 7, "rs_7.mp3"},
        {"What Goes On", "Rubber Soul", 8, "rs_8.mp3"},
        {"Girl", "Rubber Soul", 9, "rs_9.mp3"},
        {"I'm Looking Through You", "Rubber Soul", 10, "rs_10.mp3"},
        {"In My Life", "Rubber Soul", 11, "rs_11.mp3"},
        {"Wait", "Rubber Soul", 12, "rs_12.mp3"},
        {"If I Needed Someone", "Rubber Soul", 13, "rs_13.mp3"},
        {"Run for Your Life", "Rubber Soul", 14, "rs_14.mp3"},
    
        // Revolver (1966)
        {"Taxman", "Revolver", 1, "revolver_1.mp3"},
        {"Eleanor Rigby", "Revolver", 2, "revolver_2.mp3"},
        {"I'm Only Sleeping", "Revolver", 3, "revolver_3.mp3"},
        {"Love You To", "Revolver", 4, "revolver_4.mp3"},
        {"Here, There and Everywhere", "Revolver", 5, "revolver_5.mp3"},
        {"Yellow Submarine", "Revolver", 6, "revolver_6.mp3"},
        {"She Said She Said", "Revolver", 7, "revolver_7.mp3"},
        {"Good Day Sunshine", "Revolver", 8, "revolver_8.mp3"},
        {"And Your Bird Can Sing", "Revolver", 9, "revolver_9.mp3"},
        {"For No One", "Revolver", 10, "revolver_10.mp3"},
        {"Doctor Robert", "Revolver", 11, "revolver_11.mp3"},
        {"I Want to Tell You", "Revolver", 12, "revolver_12.mp3"},
        {"Got to Get You into My Life", "Revolver", 13, "revolver_13.mp3"},
        {"Tomorrow Never Knows", "Revolver", 14, "revolver_14.mp3"},
    
        // Sgt. Pepper's Lonely Hearts Club Band (1967)
        {"Sgt. Pepper's Lonely Hearts Club Band", "Sgt. Pepper's...", 1, "sgt_1.mp3"},
        {"With a Little Help from My Friends", "Sgt. Pepper's...", 2, "sgt_2.mp3"},
        {"Lucy in the Sky with Diamonds", "Sgt. Pepper's...", 3, "sgt_3.mp3"},
        {"Getting Better", "Sgt. Pepper's...", 4, "sgt_4.mp3"},
        {"Fixing a Hole", "Sgt. Pepper's...", 5, "sgt_5.mp3"},
        {"She's Leaving Home", "Sgt. Pepper's...", 6, "sgt_6.mp3"},
        {"Being for the Benefit of Mr. Kite!", "Sgt. Pepper's...", 7, "sgt_7.mp3"},
        {"Within You Without You", "Sgt. Pepper's...", 8, "sgt_8.mp3"},
        {"When I'm Sixty-Four", "Sgt. Pepper's...", 9, "sgt_9.mp3"},
        {"Lovely Rita", "Sgt. Pepper's...", 10, "sgt_10.mp3"},
        {"Good Morning Good Morning", "Sgt. Pepper's...", 11, "sgt_11.mp3"},
        {"Sgt. Pepper's Lonely Hearts Club Band (Reprise)", "Sgt. Pepper's...", 12, "sgt_12.mp3"},
        {"A Day in the Life", "Sgt. Pepper's...", 13, "sgt_13.mp3"},
    
        // The Beatles (White Album) (1968)
        {"Back in the U.S.S.R.", "The Beatles", 1, "white_1.mp3"},
        {"Dear Prudence", "The Beatles", 2, "white_2.mp3"},
        {"Glass Onion", "The Beatles", 3, "white_3.mp3"},
        {"Ob-La-Di, Ob-La-Da", "The Beatles", 4, "white_4.mp3"},
        {"Wild Honey Pie", "The Beatles", 5, "white_5.mp3"},
        {"The Continuing Story of Bungalow Bill", "The Beatles", 6, "white_6.mp3"},
        {"While My Guitar Gently Weeps", "The Beatles", 7, "white_7.mp3"},
        {"Happiness Is a Warm Gun", "The Beatles", 8, "white_8.mp3"},
        {"Martha My Dear", "The Beatles", 9, "white_9.mp3"},
        {"I'm So Tired", "The Beatles", 10, "white_10.mp3"},
        {"Blackbird", "The Beatles", 11, "white_11.mp3"},
        {"Piggies", "The Beatles", 12, "white_12.mp3"},
        {"Rocky Raccoon", "The Beatles", 13, "white_13.mp3"},
        {"Don't Pass Me By", "The Beatles", 14, "white_14.mp3"},
        {"Why Don't We Do It in the Road?", "The Beatles", 15, "white_15.mp3"},
        {"I Will", "The Beatles", 16, "white_16.mp3"},
        {"Julia", "The Beatles", 17, "white_17.mp3"},
        {"Birthday", "The Beatles", 18, "white_18.mp3"},
        {"Yer Blues", "The Beatles", 19, "white_19.mp3"},
        {"Mother Nature's Son", "The Beatles", 20, "white_20.mp3"},
        {"Everybody's Got Something to Hide Except Me and My Monkey", "The Beatles", 21, "white_21.mp3"},
        {"Sexy Sadie", "The Beatles", 22, "white_22.mp3"},
        {"Helter Skelter", "The Beatles", 23, "white_23.mp3"},
        {"Long, Long, Long", "The Beatles", 24, "white_24.mp3"},
        {"Revolution 1", "The Beatles", 25, "white_25.mp3"},
        {"Honey Pie", "The Beatles", 26, "white_26.mp3"},
        {"Savoy Truffle", "The Beatles", 27, "white_27.mp3"},
        {"Cry Baby Cry", "The Beatles", 28, "white_28.mp3"},
        {"Revolution 9", "The Beatles", 29, "white_29.mp3"},
        {"Good Night", "The Beatles", 30, "white_30.mp3"},
    
        // Yellow Submarine (1969) - Solo canciones nuevas
        {"Yellow Submarine", "Yellow Submarine", 1, "ys_1.mp3"},
        {"Only a Northern Song", "Yellow Submarine", 2, "ys_2.mp3"},
        {"All Together Now", "Yellow Submarine", 3, "ys_3.mp3"},
        {"Hey Bulldog", "Yellow Submarine", 4, "ys_4.mp3"},
        {"It's All Too Much", "Yellow Submarine", 5, "ys_5.mp3"},
        {"All You Need Is Love", "Yellow Submarine", 6, "ys_6.mp3"},
    
        // Abbey Road (1969)
        {"Come Together", "Abbey Road", 1, "ar_1.mp3"},
        {"Something", "Abbey Road", 2, "ar_2.mp3"},
        {"Maxwell's Silver Hammer", "Abbey Road", 3, "ar_3.mp3"},
        {"Oh! Darling", "Abbey Road", 4, "ar_4.mp3"},
        {"Octopus's Garden", "Abbey Road", 5, "ar_5.mp3"},
        {"I Want You (She's So Heavy)", "Abbey Road", 6, "ar_6.mp3"},
        {"Here Comes the Sun", "Abbey Road", 7, "ar_7.mp3"},
        {"Because", "Abbey Road", 8, "ar_8.mp3"},
        {"You Never Give Me Your Money", "Abbey Road", 9, "ar_9.mp3"},
        {"Sun King", "Abbey Road", 10, "ar_10.mp3"},
        {"Mean Mr. Mustard", "Abbey Road", 11, "ar_11.mp3"},
        {"Polythene Pam", "Abbey Road", 12, "ar_12.mp3"},
        {"She Came In Through the Bathroom Window", "Abbey Road", 13, "ar_13.mp3"},
        {"Golden Slumbers", "Abbey Road", 14, "ar_14.mp3"},
        {"Carry That Weight", "Abbey Road", 15, "ar_15.mp3"},
        {"The End", "Abbey Road", 16, "ar_16.mp3"},
        {"Her Majesty", "Abbey Road", 17, "ar_17.mp3"},
    
        // Let It Be (1970)
        {"Two of Us", "Let It Be", 1, "lib_1.mp3"},
        {"Dig a Pony", "Let It Be", 2, "lib_2.mp3"},
        {"Across the Universe", "Let It Be", 3, "lib_3.mp3"},
        {"I Me Mine", "Let It Be", 4, "lib_4.mp3"},
        {"Dig It", "Let It Be", 5, "lib_5.mp3"},
        {"Let It Be", "Let It Be", 6, "lib_6.mp3"},
        {"Maggie Mae", "Let It Be", 7, "lib_7.mp3"},
        {"I've Got a Feeling", "Let It Be", 8, "lib_8.mp3"},
        {"One After 909", "Let It Be", 9, "lib_9.mp3"},
        {"The Long and Winding Road", "Let It Be", 10, "lib_10.mp3"},
        {"For You Blue", "Let It Be", 11, "lib_11.mp3"},
        {"Get Back", "Let It Be", 12, "lib_12.mp3"}
    };

    cout << "Número de jugadores (1-4): ";
    int num_jugadores;
    cin >> num_jugadores;
    cin.ignore();

    vector<Jugador> jugadores;
    for(int i = 0; i < num_jugadores; ++i) {
        cout << "Nombre del jugador " << i+1 << ": ";
        string nombre;
        getline(cin, nombre);
        jugadores.emplace_back(nombre);
    }

    bool juego_activo = true;
    while(juego_activo) {
        for(auto& jugador : jugadores) {
            limpiar_pantalla();
            cout << "=== TURNO DE " << jugador.nombre << " ===\n";
            cout << "Fichas: " << jugador.fichas << " | Canciones: " 
                 << jugador.acertadas.size() << "\n\n";

            Cancion actual = canciones[rand() % canciones.size()];
            cout << "Reproduciendo canción...\n";
            reproducir_audio(actual.ruta_audio);

            bool turno_activo = true;
            bool cancion_acertada = false;

            while(turno_activo) {
                mostrar_controles();
                cout << "\nIngrese su respuesta (Título - Álbum): ";
                string entrada;
                getline(cin, entrada);

                if(to_lower(entrada) == "stop") {
                    detener_audio();
                    cout << "Música detenida\n";
                    continue;
                }

                if(to_lower(entrada) == "exit") {
                    detener_audio();
                    juego_activo = false;
                    turno_activo = false;
                    continue;
                }

                if(entrada.find("pista ") == 0) {
                    try {
                        int pista = stoi(entrada.substr(6));
                        if(pista == actual.pista) {
                            detener_audio();
                            jugador.acertadas.push_back(actual);
                            cout << "¡Posición correcta! Canción añadida.\n";
                            cancion_acertada = true;
                        } else {
                            cout << "Posición incorrecta\n";
                        }
                    } catch(...) {
                        cout << "Formato inválido\n";
                    }
                    continue;
                }


                size_t separator = entrada.find('-');
                if(separator == string::npos) {
                    cout << "Formato: Título - Álbum\n";
                    continue;
                }

                string titulo = entrada.substr(0, separator);
                string album = entrada.substr(separator + 1);

                bool acierto_titulo = (to_lower(titulo) == to_lower(actual.titulo));
                bool acierto_album = (to_lower(album) == to_lower(actual.album));

                if(acierto_titulo && acierto_album) {
                    detener_audio();
                    jugador.fichas++;
                    jugador.acertadas.push_back(actual);
                    cout << "¡Doble acierto! +1 ficha\n";
                    cancion_acertada = true;
                    turno_activo = false;
                } else if(acierto_titulo || acierto_album) {
                    cout << "Solo un acierto. ¡Necesitas ambos!\n";
                } else {
                    detener_audio();
                    cout << "\n=== CANCIÓN FALLIDA ===\n"
                         << "Título: " << actual.titulo << "\n"
                         << "Álbum: " << actual.album << "\n"
                         << "Pista: " << actual.pista << "\n"
                         << "=======================\n";
                    turno_activo = false;
                }
            }

            if(jugador.acertadas.size() >= 10) {
                cout << "\n¡" << jugador.nombre << " HA GANADO!\n";
                juego_activo = false;
            }

            if(!juego_activo) break;
            
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
        }
    }

    detener_audio();
    return 0;
}

// Compilar con: g++ -o hitster hitster.cpp -lpthread
// Ejecutar con: ./hitster