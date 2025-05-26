# BT5. Chia Sẻ Tài Nguyên Với Read-Write Lock
* Viết một chương trình mô phỏng tình huống có nhiều thread đọc và ghi dữ liệu:
    * Tạo một biến data chứa một số nguyên.
    * Tạo 5 threads đọc và 2 threads ghi:
        *  Các thread đọc sẽ chỉ đọc data mà không thay đổi.
        * Các thread ghi sẽ tăng giá trị của data lên 1.
    * Sử dụng read-write lock để đảm bảo nhiều threads có thể đọc cùng lúc, nhưng chỉ một thread được ghi tại bất kỳ thời điểm nào.
    * In ra giá trị của data sau khi tất cả các threads hoàn thành.
Gợi ý: Sử dụng pthread_rwlock_rdlock và pthread_rwlock_wrlock cho đọc-ghi đồng bộ.
