# BT4. Đếm Số Chẵn và Số Lẻ với Threads
* Viết một chương trình để thực hiện các bước sau:
    * Tạo một mảng ngẫu nhiên chứa 100 số nguyên từ 1 đến 100.
    * Tạo hai threads: một thread đếm số chẵn và một thread đếm số lẻ trong mảng.
    * Khi hai threads hoàn tất việc đếm, in ra tổng số số chẵn và tổng số số lẻ.
    * Sử dụng pthread_join để đảm bảo rằng chương trình chỉ kết thúc khi cả hai threads hoàn thành nhiệm vụ.
Gợi ý: Mỗi thread sẽ xử lý một phần công việc khác nhau, vì vậy không cần mutex trong bài này.
