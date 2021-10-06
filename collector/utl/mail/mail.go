package mail

type Service struct {
}

func New() *Service {
    return &Service{}
}

func (s *Service) SendBatteryMail(battery float64) error {
    return nil
}
