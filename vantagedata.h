#ifndef VANTAGEDATA_H
#define VANTAGEDATA_H

#include <cstdint>
#include <cmath>
#include <QtCore/QByteArray>

class VantageReader
{
public:
    enum class DataType : uint8_t
    {
        OutsideTempF,
        OutsideTempC,
        InsideTempF,
        InsideTempC,
        WindSpeedMph,
        WindSpeedKmh,
        SolarRadiationWm2,
        Power1kW,
        Power2kW,
        EnergykWh
    };

    inline VantageReader(const QByteArray& data)
    : m_pData(data)
    { if (data.size() < 52) throw std::runtime_error(""); }

    template <DataType, typename T = float>
    T value () const noexcept;

private:
    QByteArray m_pData;
};

template <>
inline float
VantageReader::value<VantageReader::DataType::OutsideTempF>() const noexcept
{
    return static_cast<float>((m_pData[12] << 8) | m_pData[13]) / 10.f;
}

template <>
inline float
VantageReader::value<VantageReader::DataType::OutsideTempC>() const noexcept
{
    return (value<DataType::OutsideTempF> () - 32) * 5 / 9;
}

template <>
inline float
VantageReader::value<VantageReader::DataType::InsideTempF>() const noexcept
{
    return static_cast<float>((m_pData[9] << 8) | m_pData[10]) / 10.f;
}

template <>
inline float
VantageReader::value<VantageReader::DataType::InsideTempC>() const noexcept
{
    return (value<DataType::InsideTempF> () - 32) * 5 / 9;
}

template <>
inline float
VantageReader::value<VantageReader::DataType::WindSpeedMph>() const noexcept
{
    return static_cast<float>(m_pData[14]);
}

template <>
inline int
VantageReader::value<VantageReader::DataType::WindSpeedMph>() const noexcept
{
    return static_cast<int>(m_pData[14]);
}

template <>
inline float
VantageReader::value<VantageReader::DataType::WindSpeedKmh>() const noexcept
{
    return value<DataType::WindSpeedMph> () * 1.6f;
}

template <>
inline int
VantageReader::value<VantageReader::DataType::WindSpeedKmh>() const noexcept
{
    return static_cast<int>(value<DataType::WindSpeedMph, float> () * 1.6f);
}

template <>
inline short
VantageReader::value<VantageReader::DataType::SolarRadiationWm2>() const noexcept
{
    return static_cast<short>((m_pData[44] << 8) | m_pData[45]);
}

template <>
inline int
VantageReader::value<VantageReader::DataType::SolarRadiationWm2>() const noexcept
{
    return static_cast<int>((m_pData[44] << 8) | m_pData[45]);
}

template <>
inline float
VantageReader::value<VantageReader::DataType::SolarRadiationWm2>() const noexcept
{
    return static_cast<float>((m_pData[44] << 8) | m_pData[45]);
}

template <>
inline double
VantageReader::value<VantageReader::DataType::Power1kW>() const noexcept
{
    return -33.1549      + 0.4204 * value<DataType::SolarRadiationWm2> ()     -
             0.00024     * std::pow(value<DataType::SolarRadiationWm2> (), 2) +
             0.000000154 * std::pow(value<DataType::SolarRadiationWm2> (), 3);
}

template <>
inline double
VantageReader::value<VantageReader::DataType::Power2kW>() const noexcept
{
    return value<DataType::SolarRadiationWm2> () * 0.00025;
}

template <>
inline float
VantageReader::value<VantageReader::DataType::EnergykWh>() const noexcept
{
    return value<DataType::SolarRadiationWm2> () * (1 / 12);
}

#endif // VANTAGEDATA_H
