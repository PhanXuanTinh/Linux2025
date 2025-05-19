#Chat Application

Chương trình dòng lệnh chat P2P

---

##1. Requirement Tính Năng (Functional Requirements)

- Giao diện dòng lệnh (Command Line Interface - CLI):
  * Ứng dụng phải hoạt động qua giao diện dòng lệnh và cung cấp các lệnh như help, myip, myport, connect, list, terminate, send, và exit.
- Hiển thị thông tin:
  * help: Hiển thị hướng dẫn sử dụng các lệnh.
  * myip: Hiển thị địa chỉ IP của máy đang chạy chương trình.
  * myport: Hiển thị port mà chương trình đang lắng nghe kết nối.
- Kết nối TCP giữa các peer:
  * connect <destination> <port>: Tạo kết nối TCP đến một peer khác qua IP và port được chỉ định.
  * Cần xử lý các trường hợp:
    * IP không hợp lệ.
    * Tự kết nối (self-connections).
    * Kết nối trùng lặp.
- Quản lý danh sách kết nối:
  * list: Hiển thị danh sách các kết nối hiện tại, bao gồm cả kết nối được tạo bởi chương trình và kết nối từ các peer khác.
- Quản lý kết nối:
  * terminate <connection id>: Ngắt kết nối với một peer dựa trên ID từ danh sách list.
  * Hiển thị lỗi nếu ID không hợp lệ hoặc kết nối không tồn tại.
- Trao đổi tin nhắn:
  * send <connection id> <message>: Gửi tin nhắn tới peer được chỉ định (ID từ danh sách list).
  * Hiển thị thông báo khi tin nhắn được gửi thành công.
  * Peer nhận tin nhắn cần hiển thị thông tin:
    * Địa chỉ IP của người gửi.
    * Port của người gửi.
    * Nội dung tin nhắn.
- Thoát khỏi ứng dụng:
  * exit: Đóng tất cả các kết nối hiện tại và thoát chương trình.
  * Các peer khác cần cập nhật danh sách kết nối để loại bỏ peer vừa thoát.
 
##2. Requirement Phi Tính Năng (Non-Functional Requirements)

- Hiệu năng:
  * Ứng dụng phải xử lý nhiều kết nối đồng thời (multi-threading hoặc asynchronous I/O).
  * Đảm bảo không bị treo (hang) khi có nhiều kết nối hoặc khi có peer ngắt kết nối đột ngột.
- Độ tin cậy:
  * Ứng dụng phải xử lý tốt các lỗi như:
    * Kết nối bị ngắt.
    * Peer không phản hồi.
    * Tin nhắn vượt quá độ dài tối đa (100 ký tự).
  * Hiển thị thông báo lỗi rõ ràng và thân thiện với người dùng.
- Không bị rò rỉ bộ nhớ (Memory Leak):
  * Đảm bảo giải phóng tài nguyên (socket, bộ nhớ) khi kết nối bị ngắt hoặc khi chương trình thoát.
  * Sử dụng các công cụ kiểm tra rò rỉ bộ nhớ (như Valgrind) nếu lập trình bằng C/C++.
- Tuân thủ quy tắc lập trình (Coding Convention):
  * Sử dụng các quy tắc đặt tên biến, hàm, cấu trúc mã nguồn rõ ràng, dễ đọc và phải nhất quán.
  * Tách biệt logic (logic xử lý) và giao diện (CLI) để dễ bảo trì.
  * Thêm comment giải thích các đoạn mã quan trọng.
- Mã nguồn sạch (Clean Code):
  * Tránh các đoạn mã lặp lại (code duplication).
  * Sử dụng các hàm/methods để chia nhỏ logic phức tạp.
  * Đặt tên biến và hàm có ý nghĩa, dễ hiểu.
  * Xử lý lỗi (error handling) đầy đủ, không để chương trình bị crash.
