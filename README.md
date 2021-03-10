# Тесты modbus

Синхронное выполнение с помощью https://docs.rs/tokio-modbus/0.4.0/tokio_modbus/index.html

## Вопросы

* Когда добавляю `#[cfg(all(feature = "tcp", feature = "sync"))]` перед методом `read_value()`, то `cfg` удаляет `read_value()`. Что-то неправильно настраиваю в `Cargo.toml`
