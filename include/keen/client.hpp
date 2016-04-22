#pragma once

#include "keen.hpp"

namespace libkeen
{

/*
 * @class Client
 * @brief The only publicly accessible Class of this
 *        library. As soon as one instance of Client
 *        is constructed, Core library is started and
 *        internal thread pool starts listening.
 * @note  Client API is thread-safe
 */
class Client
{
public:
    explicit            Client();
    virtual             ~Client();
    //! Send an event to a Metric endpoint.
    void                sendEvent(const std::string& collection, const std::string& json);
    //! Sets project ID associated with this client
    void                setProjectId(const std::string& id);
    //! Sets Write Key associated with this client
    void                setWriteKey(const std::string& key);
    //! Gets project ID associated with this client
    std::string         getProjectId() const;
    //! Gets Write Key associated with this client
    std::string         getWriteKey() const;

protected:
    //! Client sub-classes have a chance to provide their own end-point
    virtual std::string getEndpoint(const std::string& collection) const = 0;

protected:
    mutable std::mutex  mClientLock;
    CoreRef             mCoreRef;
    std::string         mProjectId;
    std::string         mWriteKey;
};

/*
 * @class ClientKeenIO
 * @brief Project ID and Write key is obtained from
 *        Keen IO's dashboard.
 */
class ClientKeenIO final : public Client
{
    std::string getEndpoint(const std::string& collection) const override;
};

/*
 * @class ClientReach
 * @brief Sends Metrics data to Helios Reach server
 */
class ClientReach final : public Client
{
    std::string getEndpoint(const std::string& collection) const override;
};

}
