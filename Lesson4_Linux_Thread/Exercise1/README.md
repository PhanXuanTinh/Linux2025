# BT1. Tạo và Chạy Threads Cơ Bản
* Viết một chương trình sử dụng thư viện pthread.h để thực hiện các thao tác sau:
    * Tạo hai threads. Mỗi thread sẽ in ra một chuỗi thông báo kèm theo ID của thread (ví dụ: "Thread 1: Hello from thread").
    * Sử dụng hàm pthread_join để đợi cả hai threads hoàn thành trước khi chương trình kết thúc.
    * Sử dụng pthread_create và pthread_join như thế nào? Khi nào thread kết thúc?
Gợi ý: Sử dụng hàm pthread_create để tạo threads và pthread_join để chờ các threads hoàn thành.