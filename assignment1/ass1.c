#include <stdio.h>
#include <string.h>

int main() {
    char text[] = "lyl qyvcu hyv tfi hmaysu mxxbi kyaxmzo, fi fmu liiz m kyaxstiv xvygvmaaiv\n\
hyv wseti m byzg teai. feu hmryveti bmzgsmgi eu kylyb, lst fi kmz mbuy\n\
xvygvma ez tfi ayjivz bmzgsmgi xotfyz. fi eu mz mkteri bimvziv qfiz ziijij.\n\
lyl tfezcu tfmt gizivmtezg mzj utyvezg m vmzjya umbt rmbsi hyv imkf suivej\n\
eu m qmuti. ezutimj, fi eu xvyxyuezg tfmt feu uoutia mjaezeutvmtyvu sui m\n\
kvoxtygvmxfek fmuf yh tfi suivej mu etu umbtu kfyekiu kivtmezbo eaxmktu tfi\n\
umbtij xmuuqyvju. lyl fmu wseti m hiq xit mzeambu, yzi jyg, yzi nilvm mzj\n\
wseti m hiq uzmciu, ezkbsjiezg tqy xotfyzu. fi byriu tfi nilvm mzj tfi jyg,\n\
lst fi fmtiu uzmciu mzj iuxikembbo xotfyzu. ykkmueyzmbbo, fi lvezgu m lyp yh\n\
kfykybmtiu ty hiij tfi nilvm. feu ceju, yz tfi ytfiv fmzj, byriu uzmciu\n\
mzj xotfyzu.\n\
\n\
tfeu eu ayvi yz lyl mzj feu hmaebo. lyl fmu hysv ceju, tfvii\n\
jmsgftivu mzj yzi uyz. feu uyz eu nimbysu mlyst ezjyyv uxyvtu,\n\
iuxikembbo lmjaeztyz. lst mbb feu jmsgftivu byri ystjyyv uxyvtu,\n\
ipkixt ezjyyv uqeaaezg. e fmri ty umo tfmt lyl eu m hmaebo gso mzj\n\
eu wseti tvsutmlbi.";

    printf("Encrypted Text\n");
    printf("--------------\n");
    printf("%s\n", text);
    for(int i = 0; i < strlen(text); i++) {
       if(text[i] == 'l') {
            text[i] = 'b';
       }
       else if(text[i] == 'b') {
            text[i] = 'l';
       }
       else if(text[i] == 'y') {
            text[i] = 'o';
       }
       else if(text[i] == 'o') {
            text[i] = 'y';
       }
       else if(text[i] == 'm') {
            text[i] = 'a';
       }
       else if(text[i] == 'a') {
            text[i] = 'm';
       }
       else if(text[i] == 'f') {
            text[i] = 'h';
       }
       else if(text[i] == 'i') {
            text[i] = 'e';
       }
       else if(text[i] == 'u') {
            text[i] = 's';
       }
       else if(text[i] == 'z') {
            text[i] = 'n';
       }
       else if(text[i] == 'n') {
            text[i] = 'z';
       }
       else if(text[i] == 'v') {
            text[i] = 'r';
       }
       else if(text[i] == 'j') {
            text[i] = 'd';
       }
       else if(text[i] == 's') {
            text[i] = 'u';
       }
       else if(text[i] == 'h') {
            text[i] = 'f';
       }
       else if(text[i] == 'e') {
            text[i] = 'i';
       }
       else if(text[i] == 'c') {
            text[i] = 'k';
       }
       else if(text[i] == 'k') {
            text[i] = 'c';
       }
       else if(text[i] == 'v') {
            text[i] = 'r';
       }
       else if(text[i] == 'r') {
            text[i] = 'v';
       }
       else if(text[i] == 'x') {
            text[i] = 'p';
       }
       else if(text[i] == 'p') {
            text[i] = 'x';
       }
       else if(text[i] == 'q') {
            text[i] = 'w';
       }
       else if(text[i] == 'w') {
            text[i] = 'q';
       }
       else {
            //do nothing
       }
    }

    printf("\nClear Text\n");
    printf("--------------\n");
    printf("%s\n", text);

    return 0;
}
