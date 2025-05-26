#BT3. Sử Dụng Condition Variables
* Viết một chương trình có hai threads: producer và consumer như sau:
    * Producer: Tạo ra một số ngẫu nhiên từ 1 đến 10 và lưu vào một biến toàn cục data.
    * Consumer: Đợi producer cung cấp dữ liệu, sau đó đọc và in ra dữ liệu đó.
    * Sử dụng pthread_cond_wait và pthread_cond_signal để đồng bộ hóa giữa producer và consumer, đảm bảo rằng consumer chỉ đọc dữ liệu khi producer đã cung cấp xong.
    * Lặp lại quá trình trên 10 lần và in ra tất cả các giá trị do consumer đọc được.
Gợi ý: Sử dụng pthread_cond_wait để cho consumer đợi cho đến khi có tín hiệu từ producer rằng dữ liệu đã sẵn sàng.