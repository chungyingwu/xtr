#ifndef XTR_ALERTER_HPP
#define XTR_ALERTER_HPP
#include <string>
#include <memory>
#include <iostream>
#include "cpr/cpr.h"
#include <nlohmann/json.hpp>
namespace xtr
{
class alerter
{
    public:

    static void create_alerter(const std::string_view alert_name, const std::string_view url) {
        if (!alerter_) {
            alerter_.reset(new alerter(alert_name, url));
        } else {
            std::cerr << "create_alerter should only be called once" << std::endl;
        }
    }

    inline __attribute__( ( always_inline ) ) static alerter* get_alerter()
    {
        return alerter_.get();
    }

    alerter(const std::string_view alert_name, const std::string_view url) : alert_name_(alert_name), alert_url_(url) 
    {

    }

    void alert(const std::string_view message) {
        using namespace std::chrono_literals;
        nlohmann::json json;
        if (!alert_name_.empty()) {
            std::string buffer;
            std::format_to(  std::back_inserter(buffer), "{} - {}", alert_name_, message);
            json["text"] = buffer;
        } else {
            json["text"] = message;
        }
        cpr::Response r = cpr::Post(cpr::Url{alert_url_},
            cpr::Timeout{2s},
            cpr::Body{nlohmann::to_string( json )},
            cpr::Header{{"Content-Type", "application/json"}});
    }

    private:
    
    inline static std::unique_ptr<alerter> alerter_;
    std::string alert_name_;
    std::string alert_url_;
    
};
}
#endif