use std::{fs::File, sync::Arc};

use log::info;
use time::macros::format_description;
use tracing::level_filters::LevelFilter;
use tracing_subscriber::{
    EnvFilter,
    fmt::{self, time::LocalTime},
    prelude::__tracing_subscriber_SubscriberExt,
    util::SubscriberInitExt,
};
pub fn setup_logging() {
    // 1. Setup the file writer
    // This will create or append to 'app.log' in the current directory
    // [TODO]: Replace with ember sim directory locator + override in param
    let log_file = File::create("app.log").expect("failed to create log file");
    let local_time = format_description!("[hour]:[minute]:[second].[subsecond digits:4]");
    let timer = LocalTime::new(local_time);
    // 2. Define the STDOUT layer (Pretty/Compact)
    let stdout_layer = fmt::layer()
        //.pretty() // Makes it human-readable with colors
        .with_file(false)
        .with_timer(timer) // Uses our HH:MM:SS format
        .with_line_number(false)
        .with_target(true); // Keeps it clean by hiding the module path

    // 3. Define the File layer
    let file_layer = fmt::layer()
        .with_ansi(false) // Don't put color codes in a text file
        .with_writer(Arc::new(log_file));

    // 4. Combine everything with the EnvFilter
    tracing_subscriber::registry()
        .with(
            EnvFilter::builder()
                .with_default_directive(LevelFilter::INFO.into())
                .from_env_lossy(),
        ) // Respects RUST_LOG
        .with(stdout_layer)
        .with(file_layer)
        .init();

    info!("Logging initialized");
}
