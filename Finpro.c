#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 100
#define MAX_EVENT_LEN 100
#define MAX_TICKET_TYPE_LEN 50
#define MAX_RECORDS 100

#define BIASA_TICKET_PRICE 25000
#define VVIP_TICKET_PRICE 50000
#define BIASA_DISCOUNT_5 100000
#define VVIP_DISCOUNT_3 125000
#define NORMAL_TICKET_PRICE 50000
#define LINE_LENGTH 120

typedef struct
{
    char transaction_date[11];
    char customer_name[MAX_NAME_LEN];
    char ticket_type[MAX_TICKET_TYPE_LEN];
    char event_name[MAX_EVENT_LEN];
    int ticket_quantity;
    float ticket_price;
    float total_payment;
    int isDiscounted;
} TicketSale;

typedef struct
{
    char customer_name[MAX_NAME_LEN];
    int has_discount;
} Customer;

TicketSale sales[MAX_RECORDS];
int total_sales = 0;

void addSale();
void sortSales();
void searchSales();
void displaySales();
void saveToFile();
void loadFromFile();
int compareByDate(const void *a, const void *b);
int compareByTotalPayment(const void *a, const void *b);
int compareByTicketQuantity(const void *a, const void *b);
void printMenu();
int isValidDate(char *date);
int isLeapYear(int year);
int getDaysInMonth(int month, int year);
int checkCustomerDiscount(char *customer_name);

int main()
{
    loadFromFile();
    int choice;

    while (1)
    {
        printMenu();
        printf("Pilih menu: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Input tidak valid!\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            addSale();
            break;
        case 2:
            sortSales();
            break;
        case 3:
            searchSales();
            break;
        case 4:
            displaySales();
            break;
        case 5:
            displayTicketPrices();
            break;
        case 6:
            printf("Keluar program...\n");
            return 0;
        default:
            printf("Pilihan tidak valid!\n");
        }
    }

    return 0;
}

void printMenu()
{
    int i;
    system("cls");

    for (i = 0; i < LINE_LENGTH; i++)
    {
        printf("=");
    }
    printf("\n");

    printf("\n                                           Menu Aplikasi Penjualan Tiket                                           \n");
    printf("\n1. Entry: Data Penjualan Tiket\n");
    printf("2. Sort: Cetak Laporan Penjualan Tiket\n");
    printf("3. Search: Cari Data Penjualan Tiket\n");
    printf("4. Tampilkan Semua Data Penjualan Tiket\n");
    printf("5. Daftar Harga Tiket\n");
    printf("6. Keluar\n\n");

    for (i = 0; i < LINE_LENGTH; i++)
    {
        printf("=");
    }
    printf("\n");
}

void addSale()
{
    int i;
    system("cls");
    if (total_sales >= MAX_RECORDS)
    {
        printf("Data penuh! Tidak bisa menambah data baru.\n");
        return;
    }

    TicketSale sale;
    int valid_date = 0;

    for (i = 0; i < LINE_LENGTH; i++)
    {
        printf("=");
    }
    printf("\n");

    while (!valid_date)
    {
        printf("\nMasukkan Tanggal Transaksi (YYYY-MM-DD): ");
        scanf("%s", sale.transaction_date);
        valid_date = isValidDate(sale.transaction_date);
        if (!valid_date)
        {
            printf("Tanggal tidak valid! Format atau nilai tanggal salah.\n");
        }
    }

    printf("Masukkan Nama Pelanggan: ");
    getchar();
    fgets(sale.customer_name, MAX_NAME_LEN, stdin);
    sale.customer_name[strcspn(sale.customer_name, "\n")] = 0;

    printf("Masukkan Jenis Tiket (BIASA / VVIP): ");
    while (1)
    {
        fgets(sale.ticket_type, MAX_TICKET_TYPE_LEN, stdin);
        sale.ticket_type[strcspn(sale.ticket_type, "\n")] = 0;

        if (strcmp(sale.ticket_type, "BIASA") == 0 || strcmp(sale.ticket_type, "VVIP") == 0)
        {
            break;
        }
        else
        {
            printf("Jenis tiket tidak valid! Silakan masukkan 'BIASA' atau 'VVIP': ");
        }
    }

    printf("Masukkan Nama Event: ");
    fgets(sale.event_name, MAX_EVENT_LEN, stdin);
    sale.event_name[strcspn(sale.event_name, "\n")] = 0;

    printf("Masukkan Jumlah Tiket: ");
    scanf("%d", &sale.ticket_quantity);

    if (strcmp(sale.ticket_type, "BIASA") == 0)
    {
        sale.ticket_price = BIASA_TICKET_PRICE;

        if (sale.ticket_quantity >= 5)
        {
            sale.total_payment = BIASA_DISCOUNT_5;
            int additional_tickets = sale.ticket_quantity - 5;
            if (additional_tickets > 0)
            {
                sale.total_payment += additional_tickets * sale.ticket_price;
            }
        }
        else
        {
            sale.total_payment = sale.ticket_quantity * sale.ticket_price;
        }
    }
    else if (strcmp(sale.ticket_type, "VVIP") == 0)
    {
        sale.ticket_price = VVIP_TICKET_PRICE;

        if (sale.ticket_quantity >= 3)
        {
            sale.total_payment = VVIP_DISCOUNT_3;
            int additional_tickets = sale.ticket_quantity - 3;
            if (additional_tickets > 0)
            {
                sale.total_payment += additional_tickets * sale.ticket_price;
            }
        }
        else
        {
            sale.total_payment = sale.ticket_quantity * sale.ticket_price;
        }
    }

    printf("\n");
    for (i = 0; i < LINE_LENGTH; i++)
    {
        printf("=");
    }
    printf("\n\n");
    sales[total_sales++] = sale;
    saveToFile();
    printf("Penjualan berhasil ditambahkan.\n");

    printf("\nTekan Enter untuk kembali ke menu...");
    getchar();
    getchar();

    system("cls");
}

int isValidDate(char *date)
{
    int year, month, day;
    if (sscanf(date, "%4d-%2d-%2d", &year, &month, &day) != 3)
    {
        return 0;
    }

    if (month < 1 || month > 12)
    {
        return 0;
    }
    int days_in_month = getDaysInMonth(month, year);
    if (day < 1 || day > days_in_month)
    {
        return 0;
    }
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    if (year > (tm_info->tm_year + 1900) ||
        (year == (tm_info->tm_year + 1900) && month > (tm_info->tm_mon + 1)) ||
        (year == (tm_info->tm_year + 1900) && month == (tm_info->tm_mon + 1) && day > tm_info->tm_mday))
    {
        return 0;
    }

    return 1;
}

int isLeapYear(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int getDaysInMonth(int month, int year)
{
    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        return isLeapYear(year) ? 29 : 28;
    default:
        return 0;
    }
}

void sortSales()
{
    int choice;
    printf("\nPilih cara pengurutan:\n");
    printf("1. Berdasarkan Tanggal Transaksi\n");
    printf("2. Berdasarkan Total Pembayaran\n");
    printf("3. Berdasarkan Jumlah Tiket\n");
    printf("Pilihan Anda: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        qsort(sales, total_sales, sizeof(TicketSale), compareByDate);
        break;
    case 2:
        qsort(sales, total_sales, sizeof(TicketSale), compareByTotalPayment);
        break;
    case 3:
        qsort(sales, total_sales, sizeof(TicketSale), compareByTicketQuantity);
        break;
    default:
        printf("Pilihan tidak valid!\n");
        return;
    }

    printf("Data telah diurutkan.\n");
    displaySales();
}

void searchSales()
{
    int choice, i;
    printf("\nPilih kriteria pencarian:\n");
    printf("1. Berdasarkan Nama Pelanggan\n");
    printf("2. Berdasarkan Jenis Tiket (BIASA/VVIP)\n");
    printf("3. Berdasarkan Nama Event\n");
    printf("Pilihan Anda: ");
    scanf("%d", &choice);

    char search_name[MAX_NAME_LEN];
    int found = 0;

    switch (choice)
    {
    case 1:
        printf("Masukkan Nama Pelanggan: ");
        getchar();
        fgets(search_name, MAX_NAME_LEN, stdin);
        search_name[strcspn(search_name, "\n")] = 0;
        break;
    case 2:
        printf("Masukkan Jenis Tiket (BIASA/VVIP): ");
        getchar();
        fgets(search_name, MAX_TICKET_TYPE_LEN, stdin);
        search_name[strcspn(search_name, "\n")] = 0;
        break;
    case 3:
        printf("Masukkan Nama Event: ");
        getchar();
        fgets(search_name, MAX_EVENT_LEN, stdin);
        search_name[strcspn(search_name, "\n")] = 0;
        break;
    default:
        printf("Pilihan tidak valid!\n");
        return;
    }

    system("cls");
    printf("\nHasil Pencarian Penjualan:\n");

    printf("+--------------+----------------------+-----------------+-------------------------+----------+----------+------------+\n");
    printf("| Tanggal      | Nama Pelanggan       | Event           | Jenis Tiket             | Jumlah   | Harga    | Total      |\n");
    printf("+--------------+----------------------+-----------------+-------------------------+----------+----------+------------+\n");

    for (i = 0; i < total_sales; i++)
    {
        if ((choice == 1 && strstr(sales[i].customer_name, search_name) != NULL) ||
            (choice == 2 && strstr(sales[i].ticket_type, search_name) != NULL) ||
            (choice == 3 && strstr(sales[i].event_name, search_name) != NULL))
        {
            printf("| %-12s | %-20s | %-15s | %-23s | %-8d | %-8.2f | %-10.2f |\n",
                   sales[i].transaction_date, sales[i].customer_name, sales[i].event_name,
                   sales[i].ticket_type, sales[i].ticket_quantity, sales[i].ticket_price, sales[i].total_payment);

            found = 1;

            if ((i + 1) % 10 == 0)
            {
                printf("+--------------+----------------------+-----------------+-------------------------+----------+----------+------------+\n");
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
                system("cls");
            }
        }
    }

    if (!found)
    {
        printf("\nTidak ada hasil pencarian yang cocok.\n");
    }
    else
    {
        printf("+--------------+----------------------+-----------------+-------------------------+----------+----------+------------+\n");
    }

    printf("\nTekan Enter untuk kembali ke menu...");
    getchar();
    system("cls");
}

void displaySales()
{
    int i;
    system("cls");

    if (total_sales == 0)
    {
        printf("Tidak ada data penjualan yang tersedia.\n");
        printf("\nTekan Enter untuk kembali ke menu...");
        getchar();
        return;
    }

    printf("\nLaporan Penjualan Tiket:\n");

    printf("+--------------+----------------------+-----------------+-------------------------+----------+----------+------------+\n");
    printf("| Tanggal      | Nama Pelanggan       | Event           | Jenis Tiket             | Jumlah   | Harga    | Total      |\n");
    printf("+--------------+----------------------+-----------------+-------------------------+----------+----------+------------+\n");

    for (i = 0; i < total_sales; i++)
    {
        printf("| %-12s | %-20s | %-15s | %-23s | %-8d | %-8.2f | %-10.2f |\n",
               sales[i].transaction_date, sales[i].customer_name, sales[i].event_name,
               sales[i].ticket_type, sales[i].ticket_quantity, sales[i].ticket_price, sales[i].total_payment);

        if ((i + 1) % 10 == 0 || i == total_sales - 1)
        {
            printf("+--------------+----------------------+-----------------+-------------------------+----------+----------+------------+\n");
            printf("\nTekan Enter untuk melanjutkan...");
            getchar();
            if (i != total_sales - 1)
                system("cls");
        }
    }
    printf("\nTekan Enter untuk kembali ke menu...");
    getchar();
    system("cls");
}

void displaySaleRow(int index)
{
    printf("| %-12s | %-20s | %-15s | %-23s | %-8d | %-8.2f | %-10.2f |\n",
           sales[index].transaction_date, sales[index].customer_name, sales[index].event_name,
           sales[index].ticket_type, sales[index].ticket_quantity, sales[index].ticket_price, sales[index].total_payment);
}

void displayTicketPrices()
{
    system("cls");

    int i;
    for (i = 0; i < LINE_LENGTH; i++)
    {
        printf("=");
    }
    printf("\n");

    printf("\n                                          ===== Daftar Harga Tiket =====                                          \n");
    printf("\n1. Tiket Biasa: Rp. %d\n", BIASA_TICKET_PRICE);
    printf("   Diskon: Pembelian 5 tiket jadi : Rp. %d\n", BIASA_DISCOUNT_5);
    printf("2. Tiket VVIP: Rp. %d\n", VVIP_TICKET_PRICE);
    printf("   Diskon: Pembelian 3 tiket jadi : Rp. %d\n", VVIP_DISCOUNT_3);
    printf("3. Tiket Normal: Rp. %d\n\n", NORMAL_TICKET_PRICE);

    for (i = 0; i < LINE_LENGTH; i++)
    {
        printf("=");
    }
    printf("\nTekan Enter untuk kembali ke menu...");
    while (getchar() != '\n')
        ;
    getchar();

    system("cls");
}

void saveToFile()
{
    int i;
    FILE *file = fopen("sales.txt", "w");
    if (file == NULL)
    {
        printf("Error membuka file!\n");
        return;
    }
    for (i = 0; i < total_sales; i++)
    {
        fprintf(file, "%s,%s,%s,%s,%d,%.2f,%.2f,%d\n",
                sales[i].transaction_date,
                sales[i].customer_name,
                sales[i].ticket_type,
                sales[i].event_name,
                sales[i].ticket_quantity,
                sales[i].ticket_price,
                sales[i].total_payment,
                sales[i].isDiscounted);
    }
    fclose(file);
}

void loadFromFile()
{
    FILE *file = fopen("sales.txt", "r");
    if (file == NULL)
    {
        printf("File tidak ditemukan. Memulai program tanpa data.\n");
        return;
    }
    while (fscanf(file, "%10s,%99[^,],%49[^,],%99[^,],%d,%f,%f,%d\n",
                  sales[total_sales].transaction_date,
                  sales[total_sales].customer_name,
                  sales[total_sales].ticket_type,
                  sales[total_sales].event_name,
                  &sales[total_sales].ticket_quantity,
                  &sales[total_sales].ticket_price,
                  &sales[total_sales].total_payment,
                  &sales[total_sales].isDiscounted) == 8)
    {
        total_sales++;
        if (total_sales >= MAX_RECORDS)
        {
            break;
        }
    }
    fclose(file);
}

int compareByDate(const void *a, const void *b)
{
    return strcmp(((TicketSale *)a)->transaction_date, ((TicketSale *)b)->transaction_date);
}

int compareByTotalPayment(const void *a, const void *b)
{
    float total_a = ((TicketSale *)a)->total_payment;
    float total_b = ((TicketSale *)b)->total_payment;
    return (total_a < total_b) - (total_a > total_b);
}

int compareByTicketQuantity(const void *a, const void *b)
{
    return ((TicketSale *)a)->ticket_quantity - ((TicketSale *)b)->ticket_quantity;
}
