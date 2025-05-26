# BT2. Đồng Bộ Hóa Threads với Mutex
* Viết một chương trình sử dụng pthread và mutex để thực hiện các thao tác sau:
    * Tạo một biến toàn cục counter khởi tạo bằng 0.
    * Tạo ba threads, mỗi thread tăng giá trị của counter lên 1, 1.000.000 lần.
    * Sử dụng mutex để đảm bảo rằng việc tăng counter được thực hiện an toàn.
    * In ra giá trị cuối cùng của counter khi tất cả các threads hoàn thành.
Hỏi: Tại sao cần mutex trong bài này? Điều gì xảy ra nếu bỏ mutex?
Gợi ý: Sử dụng pthread_mutex_lock và pthread_mutex_unlock để khóa và mở khóa mutex khi truy cập vào counter.