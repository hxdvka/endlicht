//wifi access
//rename to config.h and add to gitignore
#define _ENTP 0 // 0 personal 1 enterprise
#define _SSID   //network name
#define _PWD    //password 
#define _E_USR  //enterprise username
#define _E_PWD  //enterprise passwd

//servering setuping
#define SV_PORT 443 //443 https ; 80 http
//server to report on set up
#define SV_HOST "https://webhook.site"
#define SV_URL 
#define SV_OPT "?o=yoyo%20wake%20up%20yo"
#define SV_FP "0B 20 24 88 74 2C E4 C6 24 06 12 FC C6 8B 87 CA B9 95 AF 5C" //using clientinsecure so not required

//sv to request commands
#define SV_RNHOST "https://www.randomnumberapi.com"
#define SV_RNURL "/api/v1.0/random"

//led setup
//#define pins for lights 
#define PIN_R D7
#define PIN_G D6
#define PIN_B D5
;

//useful sites
//https://webhook.site
//https://www.randomnumberapi.com/api/v1.0/random?min=0&max=255&count=3
//https://www.randomnumberapi.com/api/v1.0/random?min=300&max=3000
