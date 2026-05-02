use log::info;
use crate::logging::setup_logging;

mod logging;

fn main() {
    setup_logging();
    info!("Logging started");
}
