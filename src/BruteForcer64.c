#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct sequence {
    char *tokens;
    int size;
    int reward;
};

struct moves {
    int *coordinates;
    int size;
    int reward;
};

int buffer_size; // >= 0
int matrix_width, matrix_height; // > 0
char *matrix;
int number_of_sequences; // >= 0
struct sequence *sequence_list;

int max_reward = 0;

void print_matrix(){
    printf("Generating Matrix...\n");
    for (int i = 0; i < matrix_height; i++){
        for (int j = 0; j < matrix_width; j++){
            printf("%c%c", matrix[(i * matrix_width + j) * 2], matrix[(i * matrix_width + j) * 2 + 1]);
            if (j < (matrix_width - 1)){printf(" ");} else {printf("\n");}
        }
    }
}

void print_sequence_list(){
    printf("Generating Sequence List...\n");
    for (int i = 0; i < number_of_sequences; i++){
        for (int j = 0; j < sequence_list[i].size; j++){
            printf("%c%c", sequence_list[i].tokens[j * 2], sequence_list[i].tokens[j * 2 + 1]);
            if (j < (sequence_list[i].size - 1)){printf(" ");} else {printf("\n");}
        }
        printf("reward: %d\n", sequence_list[i].reward);
    }
}

int pilih_mode(){
    int pilihan;
    int isInt;
    while (1){
        printf("pilih cara masukan:\n1. file .txt\n2. input teks (matriks, sekuens dan rewardnya random)\n");
        isInt = scanf("%d", &pilihan);
        if (isInt){ // mengecek apakah integer
            if ((pilihan < 1) || (pilihan > 2)){
                printf("pilih yg mana?\n");
            } else {break;}
        } else {
            while (getchar() != '\n'){} // clear input stream
            printf("pilih yg mana?\n");
        }
    }
    return pilihan;
}

int baca_txt(){
    FILE *txt_file;
    int pilihan = 1;
    char path[256] = "../txt/";
    while (1){
        printf("Masukkan nama file (di folder txt, jangan ada spasi): ");
        scanf("%248s", &path[7]);
        txt_file = fopen(path, "r");
        if (txt_file == NULL){
            printf("Gagal membaca file. Pastikan file ada dan belakangnya ada .txt\n");
            pilihan = pilih_mode();
            if (pilihan == 2){break;}
        } else {break;}
    }

    // kalau berhasil buka file .txt
    if (pilihan == 1){ // asumsi file .txt sesuai format
        fscanf(txt_file, "%d", &buffer_size);
        fscanf(txt_file, "%d %d", &matrix_width, &matrix_height);
        matrix = (char *) malloc (matrix_width * matrix_height * 2 + 1);
        for(int i = 0; i < matrix_width * matrix_height; i++){
            fscanf(txt_file, "%2s", (matrix + i * 2));
        }
        fscanf(txt_file, "%d", &number_of_sequences);
        sequence_list = (struct sequence *) malloc (number_of_sequences * sizeof(struct sequence));
        char temp = 'e'; int j;
        if (number_of_sequences > 0){while (temp != '\n'){fscanf(txt_file, "%c", &temp);}} // habisin line
        for (int i = 0; i < number_of_sequences; i++){
            sequence_list[i].tokens = (char *) malloc (buffer_size * 2 + 1); // asumsi ukuran sequence tidak mungkin lebih besar daripada ukuran buffer
            j = 0;
            while (1){
                fscanf(txt_file, "%c", &temp);
                if (temp == '\n'){break;}
                else if ((temp != ' ') && (temp != '\t')){sequence_list[i].tokens[j] = temp; j++;}
                if (j >= buffer_size * 2){break;}
            }
            sequence_list[i].size = j / 2;
            fscanf(txt_file, "%d", &(sequence_list[i].reward));
            if (i != number_of_sequences - 1){temp = 'e'; while (temp != '\n'){fscanf(txt_file, "%c", &temp);}} // habisin line
        }
        fclose(txt_file);
    }
    return pilihan;
}

void input_teks(){
    int isInt = 0;
    int jumlah_token_unik;
    int max_seq_length;

    // input
    while (!isInt){
        printf("Masukkan jumlah token unik: ");
        isInt = scanf("%d", &jumlah_token_unik);
        if (isInt && (jumlah_token_unik < 1)){printf("Harus bilangan positif!!!\n"); isInt = 0;}
        while (getchar() != '\n'){}
    }
    char token_unik[jumlah_token_unik * 2 + 1];
    int isAlfanumerik; int isUnik;
    while (1){
        printf("Masukkan token-tokennya: ");
        for (int i = 0; i < jumlah_token_unik; i++){scanf("%2s", &token_unik[i * 2]);}
        isAlfanumerik = 1; isUnik = 1;
        for (int i = 0; i < (jumlah_token_unik * 2); i++){ // cek alfanumerik
            if (((int)token_unik[i] < 48) || (((int)token_unik[i] > 57) && ((int)token_unik[i] < 65)) || (((int)token_unik[i] > 90) && ((int)token_unik[i] < 97)) || ((int)token_unik[i] > 122)){
                isAlfanumerik = 0; break;
            }
        }
        if (!isAlfanumerik){printf("harus alfanumerik!!! (hanya huruf atau angka)\n");} else {
            for (int i = 1; i < jumlah_token_unik; i++){ // cek token unik
                for (int j = 0; j < i; j++){
                    if ((token_unik[i * 2] == token_unik[j * 2]) && (token_unik[i * 2 + 1] == token_unik[j * 2 + 1])){
                        isUnik = 0; break;
                    }
                }
                if (!isUnik){break;}
            }
            if(!isUnik){printf("tidak boleh ada yang sama!!! harus unik!!!\n");} else {break;}
        }
    }
    isInt = 0;
    while (!isInt){
        printf("Masukkan ukuran buffer: ");
        isInt = scanf("%d", &buffer_size);
        if (isInt && (buffer_size < 0)){printf("Darimana ceritanya ada ukuran buffer bilangan negatif!?!?!?\n"); isInt = 0;}
        while (getchar() != '\n'){}
    }
    isInt = 0;
    while (isInt < 2){
        printf("Masukkan ukuran matriks: ");
        isInt = scanf("%d %d", &matrix_width, &matrix_height);
        if ((isInt == 2) && ((matrix_width < 1) || (matrix_height < 1))){printf("Harus lebih besar dari 0...\n"); isInt = 0;}
        while (getchar() != '\n'){}
    }
    isInt = 0;
    while (!isInt){
        printf("Masukkan jumlah sekuens: ");
        isInt = scanf("%d", &number_of_sequences);
        if (isInt && (number_of_sequences < 0)){printf("Jumlah sekuens kok minus\n"); isInt = 0;}
        while (getchar() != '\n'){}
    }
    isInt = 0;
    while (!isInt){
        printf("Masukkan ukuran maksimal sekuens: ");
        isInt = scanf("%d", &max_seq_length);
        if (isInt){
            if (max_seq_length < 2){printf("Tidak boleh dibawah 2!!!\n"); isInt = 0;}
            else if ((max_seq_length > buffer_size) && (buffer_size > 1)){printf("Tidak boleh lebih besar daripada ukuran buffer!!!\n"); isInt = 0;}
        }
    }

    // randomize.
    srand(time(NULL));
    int rn;
    matrix = (char *) malloc (matrix_width * matrix_height * 2 + 1);
    for (int i = 0; i < (matrix_width * matrix_height); i++){
        rn = rand() % jumlah_token_unik;
        matrix[i * 2] = token_unik[rn * 2];
        matrix[i * 2 + 1] = token_unik[rn * 2 + 1];
    }
    print_matrix();
    sequence_list = (struct sequence *) malloc (number_of_sequences * sizeof(struct sequence));
    for (int i = 0; i < number_of_sequences; i++){
        rn = (rand() % (max_seq_length - 1)) + 2;
        sequence_list[i].tokens = (char *) malloc (rn * 2 + 1);
        sequence_list[i].size = rn;
        for (int j = 0; j < sequence_list[i].size; j++){
            rn = rand() % jumlah_token_unik;
            sequence_list[i].tokens[j * 2] = token_unik[rn * 2];
            sequence_list[i].tokens[j * 2 + 1] = token_unik[rn * 2 + 1];
        }
        rn = rand() % 5; // 80% kemungkinan positif, 20% negatif, kelipatan 5.
        if (rn){rn = (rand() % (10 + sequence_list[i].size * 5)) + sequence_list[i].size - 1;} else {rn = (rand() % 20) - sequence_list[i].size - 19;} // interval random ditentukan juga berdasarkan ukuran sequence
        sequence_list[i].reward = rn * 5;
    }
    print_sequence_list();
}

int Check_Sekuens(struct moves check){
    int reward = 0;
    int isThere;
    for (int i = 0; i < number_of_sequences; i++){
        isThere = 0;
        if (sequence_list[i].size <= check.size){
            for (int j = 0; j <= (check.size - sequence_list[i].size); j++){
                isThere = 1;
                for (int k = 0; k < sequence_list[i].size; k++){
                    if (
                        (sequence_list[i].tokens[k * 2] !=
                        matrix[((check.coordinates[(j + k) * 2] - 1) + (check.coordinates[(j + k) * 2 + 1] - 1) * matrix_width) * 2])
                        ||
                        (sequence_list[i].tokens[k * 2 + 1] !=
                        matrix[((check.coordinates[(j + k) * 2] - 1) + (check.coordinates[(j + k) * 2 + 1] - 1) * matrix_width) * 2 + 1])
                    ){
                        isThere = 0; break;
                    }
                }
                if (isThere){break;}
            }
        }
        if(isThere){reward += sequence_list[i].reward;}
    }
    return reward;
}

void Copy_Moves(struct moves In, struct moves *Out){ // Out sudah dialokasi
    Out->size = In.size; Out->reward = In.reward;
    for (int i = 0; i < In.size; i++){
        Out->coordinates[i * 2] = In.coordinates[i * 2];
        Out->coordinates[i * 2 + 1] = In.coordinates[i * 2 + 1];
    }
}

int check_duplicate(struct moves X){ // hanya yg terakhir
    int isDupe = 0; int L = X.size - 1;
    for(int i = 0; i < L; i++){
        if ((X.coordinates[i * 2] == X.coordinates[L * 2]) && (X.coordinates[i * 2 + 1] == X.coordinates[L * 2 + 1])){isDupe = 1; break;}
    }
    return isDupe;
}

void We_Blowing_Up_Our_Computers_With_This_One(struct moves *X){
    if(X->size != buffer_size){
        struct moves Y; Y.coordinates = (int *) malloc (buffer_size * 2 * sizeof(int)); Copy_Moves(*X, &Y);
        Y.coordinates[Y.size * 2] = Y.coordinates[(Y.size - 1) * 2];
        Y.coordinates[Y.size * 2 + 1] = Y.coordinates[(Y.size - 1) * 2 + 1];
        Y.size += 1;
        struct moves temp; temp.coordinates = (int *) malloc (buffer_size * 2 * sizeof(int)); Copy_Moves(Y, &temp);
        if (X->size % 2){ // vertikal
            for (int i = 1; i <= matrix_height; i++){
                Y.coordinates[(Y.size - 1) * 2 + 1] = i;
                if (!check_duplicate(Y)){
                    Y.reward = Check_Sekuens(Y);
                    We_Blowing_Up_Our_Computers_With_This_One(&Y);
                    if (Y.reward > X->reward){Copy_Moves(Y, X); if (X->reward == max_reward){break;}}
                    Copy_Moves(temp, &Y);
                }
            }
        } else { // horizontal
            for (int i = 1; i <= matrix_width; i++){
                Y.coordinates[(Y.size - 1) * 2] = i;
                if (!check_duplicate(Y)){
                    Y.reward = Check_Sekuens(Y);
                    We_Blowing_Up_Our_Computers_With_This_One(&Y);
                    if (Y.reward > X->reward){Copy_Moves(Y, X); if (X->reward == max_reward){break;}}
                    Copy_Moves(temp, &Y);
                }
            }
        }
        free(Y.coordinates); free(temp.coordinates);
    }
}

struct moves AbsoluteSolver64(){
    struct moves result;
    result.size = 0; result.reward = 0;
    if (buffer_size < 2){return result;} // sekuens minimal dua tokens
    for (int i = 0; i < number_of_sequences; i++){ // hitung max reward
        if (sequence_list[i].reward > 0){max_reward += sequence_list[i].reward;}
    }
    if (max_reward <= 0){return result;}
    result.coordinates = (int *) malloc (buffer_size * 2 * sizeof(int));
    result.coordinates[0] = 1; result.coordinates[1] = 1; result.size = 1;
    We_Blowing_Up_Our_Computers_With_This_One(&result);
    return result;
}

void show_moves(struct moves P){
    printf("%d\n", P.reward);
    for (int i = 0; i < P.size; i++){
        printf("%c", matrix[((P.coordinates[i * 2] - 1) + (P.coordinates[i * 2 + 1] - 1) * matrix_width) * 2]);
        printf("%c", matrix[((P.coordinates[i * 2] - 1) + (P.coordinates[i * 2 + 1] - 1) * matrix_width) * 2 + 1]);
        if (i < (P.size - 1)){printf(" ");} else {printf("\n");}
    }
    for (int i = 0; i < P.size; i++){
        printf("%d, %d\n", P.coordinates[i * 2], P.coordinates[i * 2 + 1]);
    }
}

void SAVE(struct moves result, long execution_time_ms){
    char path[420] = "../test/";
    printf("Nama file yang mau disimpan (jangan ada spasi): ");
    while (getchar() != '\n'){}
    scanf("%411s", &path[8]);
    FILE *output;
    output = fopen(path, "w");
    if (path == NULL){
        // is this even possible
        printf("File gagal dibuat. Mempersiapkan rickroll...\n");
        system("START https://www.youtube.com/watch?v=dQw4w9WgXcQ");
    } else {
        fprintf(output, "%d\n", result.reward);
        for (int i = 0; i < result.size; i++){
            fprintf(output, "%c", matrix[((result.coordinates[i * 2] - 1) + (result.coordinates[i * 2 + 1] - 1) * matrix_width) * 2]);
            fprintf(output, "%c", matrix[((result.coordinates[i * 2] - 1) + (result.coordinates[i * 2 + 1] - 1) * matrix_width) * 2 + 1]);
            if (i < (result.size - 1)){fprintf(output, " ");} else {fprintf(output, "\n");}
        }
        for (int i = 0; i < result.size; i++){
            fprintf(output, "%d, %d\n", result.coordinates[i * 2], result.coordinates[i * 2 + 1]);
        }
        fprintf(output, "\n%ld ms\n", execution_time_ms);
    }
    fclose(output);
}

int main(){
    int pilihan;
    char save;
    pilihan = pilih_mode();
    if (pilihan == 1){pilihan = baca_txt();}
    if (pilihan == 2){input_teks();}

    // it's bruteforcing time
    struct timeval start, end;
    gettimeofday(&start, 0);
    struct moves result = AbsoluteSolver64();
    gettimeofday(&end, 0);
    long sec = end.tv_sec - start.tv_sec;
    long usec = end.tv_usec - start.tv_usec;
    long execution_time_ms = sec * 1000 + usec / 1000;
    show_moves(result);
    printf("\n%ld ms\n\nApakah ingin menyimpan solusi? (y/n) ", execution_time_ms);
    while (getchar() != '\n'){}
    scanf("%c", &save);
    if ((save == 'Y') || (save == 'y')){SAVE(result, execution_time_ms);} else {printf("\nUnderstandable. Have a nice day.\n");}

    return 0;
}
